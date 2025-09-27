#ifndef DECK_H
#define DECK_H
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
    [[nodiscard]] std::vector<Card> getAllCards() const;
    void updateCard(const Card& updatedCard);
    void assignCard(size_t index, const Card& card);

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
