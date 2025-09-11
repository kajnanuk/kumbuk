//
// Created by kajna on 05/09/2025.
//

#include "../include/Card.h"
#include <sstream>
#include <iomanip>
#include <ctime>

//default constructor
Card::Card() : word_(""), meaning_(""), reviewCount_(0),
               lastReviewDate_(""), easinessFactor_(2.5), interval_(1) {
}

//consturctor with parameters
Card::Card(const std::string& word, const std::string& meaning)
    : word_(word), meaning_(meaning), reviewCount_(0),
      easinessFactor_(2.5), interval_(1) {
    lastReviewDate_ = getCurrentDateString();
}

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
void Card::setReviewCount(int count) { reviewCount_ = count; }
void Card::setLastReviewDate(const std::string& date) { lastReviewDate_ = date; }
void Card::setEasinessFactor(double factor) { easinessFactor_ = factor; }
void Card::setInterval(int interval) { interval_ = interval; }

// check if card due
bool Card::isDue() const {
    if (lastReviewDate_.empty()) return true;

    // TODO
    return true;
}

// current date as string
std::string Card::getCurrentDateString() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}

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
