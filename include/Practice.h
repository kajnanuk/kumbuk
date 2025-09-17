//
// Created by kajna on 05/09/2025.
//
#ifndef PRACTICE_H
#define PRACTICE_H

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