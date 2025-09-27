#ifndef PRACTICE_H
#define PRACTICE_H
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


#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QFrame>
#include <vector>

class Deck;
class Scheduler;
class Card;

class Practice : public QWidget {
    Q_OBJECT

public:
    explicit Practice(Deck *deck, Scheduler *scheduler, QWidget *parent = nullptr);
    ~Practice() override = default;

private slots:
    void rateCard(int rating);
    void loadNextCard();

private:
    //data
    Deck *deck_;
    Scheduler *scheduler_;
    std::vector<Card> cardsForReview_;
    //current card
    Card *currentCard_;
    size_t currentCardIndex_;
    bool meaningVisible_;

    //ui
    QHBoxLayout *mainLayout_{};
    //card area
    QFrame *displayFrame_{};
    QVBoxLayout *displayLayout_{};
    QLabel *cardLabel_{};
    QLabel *instructionLabel_{};
    //buttons area
    QFrame *buttonsFrame_{};
    QVBoxLayout *buttonsLayout_{};
    QPushButton *ratingButtons_[6]{};

    //setup
    void setupUI();
    void setupDisplayArea();
    void setupRatingButtons();
    void updateCardDisplay() const;
    void showMeaning();
    void initializeCards();

    //mouse press event handling
    void mousePressEvent(QMouseEvent *event) override;

    //style
    static QString getButtonStyle();
    static QString getDisplayStyle();
    static QString getCardStyle();
    static QString getResponsiveCardStyle(int textLength);
};

#endif //PRACTICE_H