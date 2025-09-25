//
// Created by kajna on 05/09/2025.
//

#ifndef INSIGHTS_H
#define INSIGHTS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

class Deck;
class Scheduler;
class Card;

class Insights final : public QWidget {
    Q_OBJECT

public:
    explicit Insights(Deck *deck, Scheduler *scheduler, QWidget *parent = nullptr);

private slots:
    void showInsight(int statisticIndex);

private:
    //setup
    void setupUI();
    void setupDisplayArea();
    void setupInsightButtons();

    //all the insights tabs
    void showOverview(const std::vector<Card> & cards) const;
    void showReview(const std::vector<Card> & cards) const;
    void showInterval(const std::vector<Card> & cards) const;
    void showQuality(const std::vector<Card> & cards) const;
    void showEasiness(const std::vector<Card> & cards) const;
    void showVocabulary(const std::vector<Card> & cards) const;

    //update display
    void updateStatisticDisplay() const;

    // style
    static QString getButtonStyle() ;
    static QString getDisplayStyle() ;
    static QString getInsightLabelStyle() ;

private:
    //data
    Deck *deck_;
    Scheduler *scheduler_;

    //ui
    QFrame *displayFrame_{};
    QFrame *buttonsFrame_{};
    QLabel *titleLabel_{};
    QLabel *insightLabel_{};
    QLabel *instructionLabel_{};
    QPushButton *insightButtons_[6]{};

    //state
    int currentInsight_;
};

#endif //INSIGHTS_H
