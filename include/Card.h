#ifndef CARD_H
#define CARD_H
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


#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Card {
public:
    // constructors
    Card();
    Card(std::string  word, std::string  meaning);

    //getters
    [[nodiscard]] const std::string& getWord() const;
    [[nodiscard]] const std::string& getMeaning() const;
    [[nodiscard]] int getReviewCount() const;
    [[nodiscard]] const std::string& getLastReviewDate() const;
    [[nodiscard]] double getEasinessFactor() const;
    [[nodiscard]] int getInterval() const;

    //setters
    void setWord(const std::string& word);
    void setMeaning(const std::string& meaning);
    void setReviewCount(int count);
    void setLastReviewDate(const std::string& date);
    void setEasinessFactor(double factor);
    void setInterval(int interval);


private:
    std::string word_;
    std::string meaning_;
    int reviewCount_;
    std::string lastReviewDate_;
    double easinessFactor_;  // for sm-2 (default 2.5)
    int interval_;           //days until next review
};

//json serialisation
void to_json(json& j, const Card& card);
void from_json(const json& j, Card& card);

#endif // CARD_H
