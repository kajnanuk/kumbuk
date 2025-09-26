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

#include "../include/Practice.h"
#include "../include/Deck.h"
#include "../include/Scheduler.h"
#include "../include/Card.h"

Practice::Practice(Deck *deck, Scheduler *scheduler, QWidget *parent)
    : QWidget(parent), deck_(deck), scheduler_(scheduler),
      currentCard_(nullptr), currentCardIndex_(0), meaningVisible_(false) {

    setupUI();
    initializeCards();
}

void Practice::setupUI() {
    //horizontal layout
    mainLayout_ = new QHBoxLayout(this);
    mainLayout_->setContentsMargins(0, 0, 0, 0);
    mainLayout_->setSpacing(2);

    setupDisplayArea();
    setupRatingButtons();
}

void Practice::setupDisplayArea() {
    //big block
    displayFrame_ = new QFrame();
    displayFrame_->setFrameStyle(QFrame::Box);
    displayFrame_->setStyleSheet(getDisplayStyle());
    displayFrame_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //display layout
    displayLayout_ = new QVBoxLayout(displayFrame_);
    displayLayout_->setAlignment(Qt::AlignCenter);
    displayLayout_->setContentsMargins(10, 10, 10, 10);
    displayLayout_->setSpacing(20);

    //card content
    cardLabel_ = new QLabel("nothing to review");
    cardLabel_->setAlignment(Qt::AlignCenter);
    cardLabel_->setStyleSheet(getCardStyle());
    cardLabel_->setWordWrap(true);
    cardLabel_->setScaledContents(false);
    cardLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cardLabel_->setMinimumSize(100, 50);

    //instruction text
    instructionLabel_ = new QLabel("right-click to reveal");
    instructionLabel_->setAlignment(Qt::AlignCenter);
    instructionLabel_->setStyleSheet(
        "QLabel {"
        " color: #888888;"
        " font-size: 20px;"
        " border: 1px solid #888888;"
        " padding: 10px;"
        " font-weight: bold;"
        "}"
    );

    //displayLayout_->addStretch();
    displayLayout_->addWidget(cardLabel_);
    displayLayout_->addWidget(instructionLabel_);
    //displayLayout_->addStretch();

    mainLayout_->addWidget(displayFrame_, 3); //big block takes up 0.75 of space
}

void Practice::setupRatingButtons() {
    //frame for buttons
    buttonsFrame_ = new QFrame();
    buttonsFrame_->setFrameStyle(QFrame::Box);
    buttonsFrame_->setStyleSheet(
        "QFrame {"
        "  border: 1px solid white;"
        "  border-radius: 0px;"
        "}"
    );
    buttonsFrame_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //buttons layout
    buttonsLayout_ = new QVBoxLayout(buttonsFrame_);
    buttonsLayout_->setContentsMargins(10, 10, 10, 10);
    buttonsLayout_->setSpacing(2);

    //button labels
    const QStringList buttonLabels = {
        "perfect",
        "good",
        "okay",
        "bad",
        "awful",
        "sad"
    };

    //button style and connection
    for (int i = 0; i < 6; i++) {
        ratingButtons_[i] = new QPushButton(buttonLabels[i]);
        ratingButtons_[i]->setStyleSheet(getButtonStyle());
        ratingButtons_[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ratingButtons_[i]->setEnabled(false); //disabled until card revealed

        //connect to slot
        connect(ratingButtons_[i], &QPushButton::clicked, this, [this, i]() {
            rateCard(6-i); //reversed
        });

        buttonsLayout_->addWidget(ratingButtons_[i]);
    }

    //add to main layout
    mainLayout_->addWidget(buttonsFrame_, 1); //frame takes up 0.25 of space
}

void Practice::initializeCards() {
    if (deck_ && scheduler_) {
        const auto allCards = deck_->getAllCards(); //first get all cards
        cardsForReview_ = Scheduler::getCardsForReview(allCards); //then ask scheduler to get the ones for review
        currentCardIndex_ = 0;

        if (!cardsForReview_.empty()) {
            loadNextCard();
        } else {
            cardLabel_->setText("no cards to review right now");
            instructionLabel_->setText("add more cards or come back later");
        }
    } else {
        cardLabel_->setText("no deck loaded");
        instructionLabel_->setText("load a deck first");
    }
}

void Practice::loadNextCard() {
    if (cardsForReview_.empty() || currentCardIndex_ >= cardsForReview_.size()) {
        cardLabel_->setText("review session complete");
        instructionLabel_->setText("all cards reviewed for today");

        //disable rating buttons
        for (const auto & ratingButton : ratingButtons_) {
            ratingButton->setEnabled(false);
        }
        return;
    }

    //load front of card
    currentCard_ = &cardsForReview_[currentCardIndex_];
    meaningVisible_ = false;

    //refresh
    updateCardDisplay();
}

void Practice::updateCardDisplay() const {
    if (!currentCard_) return;

    if (!meaningVisible_) {
        //disable rating buttons
        for (const auto & ratingButton : ratingButtons_) {
            ratingButton->setEnabled(false);
        }
        //show front
        const QString word = QString::fromStdString(currentCard_->getWord());

        //"responsive' style
        const QString dynamicStyle = getResponsiveCardStyle(word.length());
        cardLabel_->setStyleSheet(dynamicStyle);
        cardLabel_->setText(word);

        //instruction
        instructionLabel_->setText("right-click to reveal");
    } else {
        //enable rating buttons
        for (const auto & ratingButton : ratingButtons_) {
            ratingButton->setEnabled(true);
        }
        //show both front and back
        const QString word = QString::fromStdString(currentCard_->getWord());
        const QString meaning = QString::fromStdString(currentCard_->getMeaning());
        const QString combined = word + "\n\n" + meaning;

        //"responsive" style for full length
        const QString dynamicStyle = getResponsiveCardStyle(combined.length());
        cardLabel_->setStyleSheet(dynamicStyle);
        cardLabel_->setText(combined);

        //instruction
        instructionLabel_->setText("rate your recall");
    }
}

void Practice::showMeaning() {
    if (currentCard_ && !meaningVisible_) {
        meaningVisible_ = true;
        updateCardDisplay();
    }
}

void Practice::rateCard(const int rating) {
    if (!currentCard_ || !meaningVisible_) {
        return; //rate only after revealing
    }

    //rating to Quality enum
    Scheduler::Quality quality;
    switch (rating) {
        case 1: quality = Scheduler::Quality::COMPLETE_BLACKOUT; break;
        case 2: quality = Scheduler::Quality::INCORRECT_DIFFICULT; break;
        case 3: quality = Scheduler::Quality::INCORRECT_EASY; break;
        case 4: quality = Scheduler::Quality::CORRECT_DIFFICULT; break;
        case 5: quality = Scheduler::Quality::CORRECT_HESITANT; break;
        case 6: quality = Scheduler::Quality::PERFECT; break;
        default: return;
    }

    //update card schedule
    Scheduler::updateCardSchedule(*currentCard_, quality);

    //update card in deck
    deck_->updateCard(*currentCard_);

    //go to next card
    currentCardIndex_++;
    loadNextCard();
}

void Practice::mousePressEvent(QMouseEvent *event) {
    //show meaning when right-clicked
    if (event->button() == Qt::RightButton && currentCard_ && !meaningVisible_) {
        showMeaning();
    }

    QWidget::mousePressEvent(event);
}

QString Practice::getButtonStyle() {
    return
        "QPushButton {"
        "  color: white;"
        "  border: 1px solid white;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  border-radius: 0px;"
        "  text-align: center;"
        "}"
        "QPushButton:hover {"
        "  background-color: #9900bb;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #0000ff;"
        "}"
        "QPushButton:disabled {"
        "  color: #444444;"
        "  border-color: #444444;"
        "}";
}

QString Practice::getDisplayStyle() {
    return
        "QFrame {"
        "  border: 1px solid white;"
        "  border-radius: 0px;"
        "}";
}


QString Practice::getCardStyle() {
    return
        "QLabel {"
        "  color: white;"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  border: none;"
        "}";
}

QString Practice::getResponsiveCardStyle(int textLength) {
    int fontSize = 22; //default

    //adjust based on content length
    if (textLength > 100) {
        fontSize = 17;
    } else if (textLength > 50) {
        fontSize = 18;
    } else if (textLength > 30) {
        fontSize = 19;
    }

    return QString(
        "QLabel {"
        "  color: white;"
        "  font-size: %1px;"
        "  font-weight: bold;"
        "  border: none;"
        "}"
    ).arg(fontSize);
}
