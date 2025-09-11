//
// Created by kajna on 05/09/2025.
//

#ifndef DECK_H
#define DECK_H

#include <vector>
#include <string>
#include "Card.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Deck {
public:
    //constructor
    Deck();
    Deck(const std::string& deckName);

    //managing cards
    void addCard(const Card& card);
    void removeCard(size_t index);
    Card& getCard(size_t index);
    const Card& getCard(size_t index) const;
    size_t getCardCount() const;

    //deck getters and setters
    const std::string& getDeckName() const;
    void setDeckName(const std::string& name);

    // managing files
    bool loadFromFile(const std::string& filepath);
    bool saveToFile(const std::string& filepath) const;

    //other
    std::vector<Card> getDueCards() const;
    void clear();
    bool isEmpty() const;

    // iterator
    std::vector<Card>::iterator begin();
    std::vector<Card>::iterator end();
    std::vector<Card>::const_iterator begin() const;
    std::vector<Card>::const_iterator end() const;

private:
    std::string deckName_;
    std::vector<Card> cards_;
};

#endif // DECK_H
