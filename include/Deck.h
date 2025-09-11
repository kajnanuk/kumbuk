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
    explicit Deck(std::string  deckName);

    //managing cards
    void addCard(const Card& card);
    void removeCard(int index);
    Card& getCard(size_t index);
    [[nodiscard]] const Card& getCard(size_t index) const;
    [[nodiscard]] size_t getCardCount() const;

    //deck getters and setters
    [[nodiscard]] const std::string& getDeckName() const;
    void setDeckName(const std::string& name);

    // managing files
    bool loadFromFile(const std::string& filepath);
    [[nodiscard]] bool saveToFile(const std::string& filepath) const;

    //other
    void clear();
    [[nodiscard]] bool isEmpty() const;

    // iterator
    std::vector<Card>::iterator begin();
    std::vector<Card>::iterator end();
    [[nodiscard]] std::vector<Card>::const_iterator begin() const;
    [[nodiscard]] std::vector<Card>::const_iterator end() const;

private:
    std::string deckName_;
    std::vector<Card> cards_;
};

#endif // DECK_H
