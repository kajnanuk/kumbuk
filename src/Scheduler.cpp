/*
* Copyright (c) 2025 kajnanuk
 * Licensed under LGPL v3
 *
 * This application uses:
 * - Qt Framework (LGPL v3) - Copyright The Qt Company Ltd.
 * - nlohmann/json (MIT) - Copyright © 2013-2025 Niels Lohmann
 * - SM-2 Algorithm - Algorithm SM-2, (C) Copyright SuperMemo World, 1991.
 *
 * Full license information available in the project's LICENSE and THIRD-PARTY-LICENSES files.
 */


#include "../include/Scheduler.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>

//constructor
Scheduler::Scheduler() = default;

//sm-2 methods
void Scheduler::updateCardSchedule(Card& card, Quality quality) {
    int qualityValue = static_cast<int>(quality);

    //update review count
    card.setReviewCount(card.getReviewCount() + 1);
    card.setLastReviewDate(getCurrentDateString());

    //calculate ease factor
    double newEF = calculateNewEasinessFactor(card.getEasinessFactor(), quality);
    card.setEasinessFactor(newEF);

    //calculate next interval
    int newInterval = calculateNextInterval(card, quality, newEF);
    card.setInterval(newInterval);

    std::cout << "Updated card '" << card.getWord() << "':" << std::endl;
    std::cout << "  Quality: " << qualityValue << std::endl;
    std::cout << "  New EF: " << newEF << std::endl;
    std::cout << "  Next interval: " << newInterval << " days" << std::endl;
}

int Scheduler::calculateNextInterval(const Card& card, Quality quality, double newEasinessFactor) {
    int reviewCount = card.getReviewCount();
    int qualityValue = static_cast<int>(quality);

    if (qualityValue < 3) {
        return 1; //if too bad, start again
    }
    if (reviewCount == 1) {
        return 1; //first after 1 dat
    }
    if (reviewCount == 2) {
        return 6; //second after 6 days
    }
    //later: interval = previous_interval * ease_factor
    return static_cast<int>(std::ceil(card.getInterval() * newEasinessFactor));

}

double Scheduler::calculateNewEasinessFactor(double currentEF, Quality quality) {
    int qualityValue = static_cast<int>(quality);

    // sm-2 formula: EF' = EF + (0.1 - (5-q) * (0.08 + (5-q) * 0.02))
    double newEF = currentEF + (0.1 - (5 - qualityValue) * (0.08 + (5 - qualityValue) * 0.02));

    // min ease factor = 1.3
    if (newEF < 1.3) {
        newEF = 1.3;
    }

    return newEF;
}

//date logic
std::string Scheduler::getCurrentDateString() {
    auto now = std::chrono::system_clock::now();
    return timePointToString(now);
}

std::string Scheduler::getDateAfterDays(const std::string& startDate, int days) {
    auto startTime = stringToTimePoint(startDate);
    auto targetTime = startTime + std::chrono::hours(24 * days);
    return timePointToString(targetTime);
}

bool Scheduler::isCardDue(const Card& card) {
    if (card.getLastReviewDate().empty()) {
        return false; //card is new
    }

    std::string currentDate = getCurrentDateString();
    std::string dueDate = getDateAfterDays(card.getLastReviewDate(), card.getInterval());

    return currentDate >= dueDate;
}

int Scheduler::daysBetweenDates(const std::string& date1, const std::string& date2) {
    auto time1 = stringToTimePoint(date1);
    auto time2 = stringToTimePoint(date2);

    auto duration = time2 - time1;
    auto days = std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;

    return static_cast<int>(days);
}

//managing deck
std::vector<Card> Scheduler::getDueCards(const std::vector<Card>& allCards) {
    std::vector<Card> dueCards;

    for (const auto& card : allCards) {
        if (isCardDue(card)) {
            dueCards.push_back(card);
        }
    }

    return dueCards;
}

std::vector<Card> Scheduler::getNewCards(const std::vector<Card>& allCards, int limit) {
    std::vector<Card> newCards;
    int count = 0;

    for (const auto& card : allCards) {
        if (card.getReviewCount() == 0 && count < limit) {
            newCards.push_back(card);
            count++;
        }
    }

    return newCards;
}

std::vector<Card> Scheduler::getCardsForReview(const std::vector<Card>& allCards) {
    std::vector<Card> reviewCards;

    //get due cards
    auto dueCards = getDueCards(allCards);
    reviewCards.insert(reviewCards.end(), dueCards.begin(), dueCards.end());

    //add to queue some cards, limit to 10
    auto newCards = getNewCards(allCards, 10);
    reviewCards.insert(reviewCards.end(), newCards.begin(), newCards.end());

    //shuffle cards
    std::random_device rd;
    std::mt19937 g(rd());
    std::ranges::shuffle(reviewCards, g);

    return reviewCards;
}

//time methods
std::chrono::system_clock::time_point Scheduler::stringToTimePoint(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss>>std::get_time(&tm, "%Y-%m-%d");

    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::string Scheduler::timePointToString(const std::chrono::system_clock::time_point& tp) {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    auto tm = *std::localtime(&time_t);

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}
