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


#include "../include/Card.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <utility>

//default constructor
Card::Card() : reviewCount_(0),easinessFactor_(2.5), interval_(1) {
}

//consturctor with parameters
Card::Card(std::string  word, std::string  meaning)
    : word_(std::move(word)), meaning_(std::move(meaning)), reviewCount_(0),
      easinessFactor_(2.5), interval_(1) {}

//getters
const std::string& Card::getWord() const { return word_; }
const std::string& Card::getMeaning() const { return meaning_; }
int Card::getReviewCount() const { return reviewCount_; }
const std::string& Card::getLastReviewDate() const { return lastReviewDate_; }
double Card::getEasinessFactor() const { return easinessFactor_; }
int Card::getInterval() const { return interval_; }

//setters
void Card::setWord(const std::string& word) { word_ = word; }
void Card::setMeaning(const std::string& meaning) { meaning_ = meaning; }
void Card::setReviewCount(const int count) { reviewCount_ = count; }
void Card::setLastReviewDate(const std::string& date) { lastReviewDate_ = date; }
void Card::setEasinessFactor(const double factor) { easinessFactor_ = factor; }
void Card::setInterval(const int interval) { interval_ = interval; }

//json serialisation
void to_json(json& j, const Card& card) {
    j = json{
        {"word", card.getWord()},
        {"meaning", card.getMeaning()},
        {"reviewCount", card.getReviewCount()},
        {"lastReviewDate", card.getLastReviewDate()},
        {"easinessFactor", card.getEasinessFactor()},
        {"interval", card.getInterval()}
    };
}

void from_json(const json& j, Card& card) {
    card.setWord(j.at("word").get<std::string>());
    card.setMeaning(j.at("meaning").get<std::string>());
    card.setReviewCount(j.at("reviewCount").get<int>());
    card.setLastReviewDate(j.at("lastReviewDate").get<std::string>());
    card.setEasinessFactor(j.at("easinessFactor").get<double>());
    card.setInterval(j.at("interval").get<int>());
}
