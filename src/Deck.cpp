//
// Created by kajna on 05/09/2025.
//

#include "../include/Deck.h"
#include <fstream>
#include <iostream>

//constructors
Deck::Deck() : deckName_("Untitled Deck") {
}

Deck::Deck(const std::string& deckName) : deckName_(deckName) {
}

//managing card
void Deck::addCard(const Card& card) {
    cards_.push_back(card);
}

void Deck::removeCard(size_t index) {
    if (index < cards_.size()) {
        cards_.erase(cards_.begin() + index);
    }
}

Card& Deck::getCard(size_t index) {
    return cards_.at(index);
}

const Card& Deck::getCard(size_t index) const {
    return cards_.at(index);
}

size_t Deck::getCardCount() const {
    return cards_.size();
}

//deck props
const std::string& Deck::getDeckName() const {
    return deckName_;
}

void Deck::setDeckName(const std::string& name) {
    deckName_ = name;
}

//files
bool Deck::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for reading: " << filepath << std::endl;
        return false;
    }

    try {
        json j;
        file >> j;

        //getting deck name
        if (j.contains("deckName")) {
            deckName_ = j["deckName"].get<std::string>();
        }

        //getting cards
        cards_.clear();
        if (j.contains("cards") && j["cards"].is_array()) {
            for (const auto& cardJson : j["cards"]) {
                Card card;
                from_json(cardJson, card);
                cards_.push_back(card);
            }
        }

        std::cout << "Successfully loaded deck: " << deckName_
                  << " with " << cards_.size() << " cards" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
        return false;
    }
}

bool Deck::saveToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
        return false;
    }

    try {
        json j;
        j["deckName"] = deckName_;
        j["cards"] = json::array();

        for (const auto& card : cards_) {
            json cardJson;
            to_json(cardJson, card);
            j["cards"].push_back(cardJson);
        }

        file << j.dump(4);  //printing with indent

        std::cout << "Successfully saved deck: " << deckName_
                  << " with " << cards_.size() << " cards" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error writing JSON file: " << e.what() << std::endl;
        return false;
    }
}

//other methods
std::vector<Card> Deck::getDueCards() const {
    std::vector<Card> dueCards;
    for (const auto& card : cards_) {
        if (card.isDue()) {
            dueCards.push_back(card);
        }
    }
    return dueCards;
}

void Deck::clear() {
    cards_.clear();
}

bool Deck::isEmpty() const {
    return cards_.empty();
}

//iterator
std::vector<Card>::iterator Deck::begin() { return cards_.begin(); }
std::vector<Card>::iterator Deck::end() { return cards_.end(); }
std::vector<Card>::const_iterator Deck::begin() const { return cards_.begin(); }
std::vector<Card>::const_iterator Deck::end() const { return cards_.end(); }

