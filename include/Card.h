//
// Created by kajna on 05/09/2025.
//

#ifndef CARD_H
#define CARD_H

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Card {
public:
    // constructors
    Card();
    Card(const std::string& word, const std::string& meaning);

    //getters
    const std::string& getWord() const;
    const std::string& getMeaning() const;
    int getReviewCount() const;
    const std::string& getLastReviewDate() const;
    double getEasinessFactor() const;
    int getInterval() const;

    //setters
    void setWord(const std::string& word);
    void setMeaning(const std::string& meaning);
    void setReviewCount(int count);
    void setLastReviewDate(const std::string& date);
    void setEasinessFactor(double factor);
    void setInterval(int interval);

    //other
    bool isDue() const;
    std::string getCurrentDateString() const;

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
