#ifndef SCHEDULER_H
#define SCHEDULER_H
/*
 * Copyright (c) 2025 kajnanuk
 * Licensed under LGPL v3
 *
 * This application uses:
 * - Qt Framework (LGPL v3) - Copyright The Qt Company Ltd.
 * - nlohmann/json (MIT) - Copyright © 2013-2025 Niels Lohmann
 * - SM-2 Algorithm - Algorithm SM-2, (C) Copyright SuperMemo World, 1991.
 * - Wikipedia content (CC BY-SA 4.0) – “Wikipedia contributors”
 *
 * Full license information available in the project's LICENSE and THIRD-PARTY-LICENSES files.
 */


#include "Card.h"
#include <vector>
#include <chrono>
#include <string>

class Scheduler {
public:
    //quality ratings for sm-2
    enum class Quality {
        COMPLETE_BLACKOUT    = 0,
        INCORRECT_DIFFICULT  = 1,
        INCORRECT_EASY       = 2,
        CORRECT_DIFFICULT    = 3,
        CORRECT_HESITANT     = 4,
        PERFECT              = 5
    };


    //constructor
    Scheduler();

    //sm-2 methods
    static void updateCardSchedule(Card& card, Quality quality) ;

    static int calculateNextInterval(const Card& card, Quality quality, double newEasinessFactor);

    static double calculateNewEasinessFactor(double currentEF, Quality quality);

    //date and scheduling
    static std::string getCurrentDateString() ;
    static std::string getDateAfterDays(const std::string& startDate, int days) ;
    static bool isCardDue(const Card& card) ;
    static int daysBetweenDates(const std::string& date1, const std::string& date2) ;

    //managing deck w/ scheduling
    static std::vector<Card> getDueCards(const std::vector<Card>& allCards) ;
    static std::vector<Card> getNewCards(const std::vector<Card>& allCards, int limit = 10) ;

    static std::vector<Card> getCardsForReview(const std::vector<Card>& allCards);

private:
    // time methods
    static std::chrono::system_clock::time_point stringToTimePoint(const std::string& dateStr);
    static std::string timePointToString(const std::chrono::system_clock::time_point& tp);
};

#endif // SCHEDULER_H

