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


#include "../include/Insights.h"
#include "../include/Deck.h"
#include "../include/Scheduler.h"

Insights::Insights(Deck *deck, Scheduler *scheduler, QWidget *parent)
: QWidget(parent), deck_(deck), scheduler_(scheduler), currentInsight_(0) {
    setupUI();
    updateStatisticDisplay();
}

//setup
void Insights::setupUI() {
    //main layout
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(2);

    //display area and buttons setup
    setupDisplayArea();
    setupInsightButtons();

    //ratio and add to main layout
    mainLayout->addWidget(displayFrame_, 3); //0.75 of space
    mainLayout->addWidget(buttonsFrame_, 1); //0.25 of space
}

void Insights::setupDisplayArea() {
    //display frame
    displayFrame_ = new QFrame();
    displayFrame_->setFrameStyle(QFrame::Box);
    displayFrame_->setStyleSheet(getDisplayStyle());
    displayFrame_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //display layout
    auto *displayLayout = new QVBoxLayout(displayFrame_);
    displayLayout->setAlignment(Qt::AlignCenter);
    displayLayout->setContentsMargins(10, 10, 10, 10);
    displayLayout->setSpacing(2);

    //title label
    titleLabel_ = new QLabel("deck insights");
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("font-size: 20px; color: white; font-weight: bold; padding: 10px;");
    titleLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    //main insights label
    insightLabel_ = new QLabel("select an insights category");
    insightLabel_->setAlignment(Qt::AlignCenter);
    insightLabel_->setStyleSheet(getInsightLabelStyle());
    insightLabel_->setWordWrap(true);
    insightLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    insightLabel_->setMinimumSize(200, 100);

    //instruction label
    instructionLabel_ = new QLabel("choose from the options on the right");
    instructionLabel_->setAlignment(Qt::AlignCenter);
    instructionLabel_->setStyleSheet("font-size: 20px; color: #888888; padding: 10px; border: 1px solid #888888; font-weight: bold;");
    instructionLabel_->setWordWrap(true);

    //add to layout
    displayLayout->addWidget(titleLabel_);
    displayLayout->addStretch();
    displayLayout->addWidget(insightLabel_);
    displayLayout->addStretch();
    displayLayout->addWidget(instructionLabel_);
}

void Insights::setupInsightButtons() {
    //button frame
    buttonsFrame_ = new QFrame();
    buttonsFrame_->setFrameStyle(QFrame::Box);
    buttonsFrame_->setStyleSheet("QFrame { border: 1px solid white; border-radius: 0px; }");
    buttonsFrame_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //button layout
    auto *buttonsLayout = new QVBoxLayout(buttonsFrame_);
    buttonsLayout->setContentsMargins(10, 10, 10, 10);
    buttonsLayout->setSpacing(2);

    //button labels
    const QStringList buttonLabels = {
        "overview",
        "review",
        "interval",
        "quality",
        "easiness",
        "vocabulary"
    };

    //create buttons
    for (int i = 0; i < 6; i++) {
        insightButtons_[i] = new QPushButton(buttonLabels[i]);
        insightButtons_[i]->setStyleSheet(getButtonStyle());
        insightButtons_[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        //connect buttons
        connect(insightButtons_[i], &QPushButton::clicked, this, [this, i]() {
            showInsight(i);
        });
        buttonsLayout->addWidget(insightButtons_[i]);
    }
}

//helpers
void Insights::showInsight(const int statisticIndex) {
    currentInsight_ = statisticIndex;
    updateStatisticDisplay();
}

void Insights::updateStatisticDisplay() const {
    //deck not loaded
    if (!deck_ || !scheduler_) {
        insightLabel_->setText("no deck loaded");
        instructionLabel_->setText("load a deck to view insights");
        return;
    }

    const auto allCards = deck_->getAllCards();

    //empty deck
    if (allCards.empty()) {
        insightLabel_->setText("empty deck");
        instructionLabel_->setText("add cards to view insights");
        return;
    }

    //show chosen insight
    switch (currentInsight_) {
        case 0:
            showOverview(allCards);
            break;
        case 1:
            showReview(allCards);
            break;
        case 2:
            showInterval(allCards);
            break;
        case 3:
            showQuality(allCards);
            break;
        case 4:
            showEasiness(allCards);
            break;
        case 5:
            showVocabulary(allCards);
            break;
        default:
            insightLabel_->setText("select an insights category");
            instructionLabel_->setText("choose from the options on the right");
            break;
    }
    enableButtons(currentInsight_);

    // update title
    const QStringList titles = {
        "overview",
        "review",
        "interval",
        "quality",
        "easiness",
        "vocabulary"
    };
    if (currentInsight_ >= 0 && currentInsight_ < titles.size()) {
        titleLabel_->setText(titles[currentInsight_]);
    }
}

void Insights::enableButtons(const int currentInsight) const {
    insightButtons_[currentInsight]->setEnabled(false);
    for (int i=0;i<6;i++) {
        if (i!=currentInsight) {
            insightButtons_[i]->setEnabled(true);
        }
    }
}

//insight tabs
void Insights::showOverview(const std::vector<Card>& cards) const {
    const int totalCards = static_cast<int>(cards.size());
    int newCards = 0;
    int reviewedCards = 0;
    int dueCards = 0;

    for (const auto& card : cards) {
        if (card.getReviewCount() == 0) {
            newCards++;
        } else {
            reviewedCards++;
            if (Scheduler::isCardDue(card)) {
                dueCards++;
            }
        }
    }

    const int totalToStudy = dueCards + std::min(newCards, 10);

    //set text for insight label
    const QString overview = QString(
        "number of cards: %1\n"
        "reviewed: %2\n"
        "new: %3\n\n"

        "due today: %4\n"
        "new to learn today: %5\n"
        "total to study today: %6"
    ).arg(totalCards).arg(reviewedCards).arg(newCards)
     .arg(dueCards).arg(std::min(newCards, 10)).arg(totalToStudy);

    //change labels
    insightLabel_->setText(overview);
    instructionLabel_->setText("quick overview of deck");
}

void Insights::showReview(const std::vector<Card>& cards) const {
    Card mostRecentCard, oldestCard;
    std::string mostRecentDate;
    std::string oldestDate = "9999-12-31";
    bool hasReviewed = false;

    int totalReviews = 0;
    int maxReviews = 0;
    int neverReviewed = 0;

    for (const auto& card : cards) {
        totalReviews += card.getReviewCount();
        maxReviews = std::ranges::max(card.getReviewCount(), maxReviews);
        neverReviewed += card.getReviewCount() == 0 ? 1 : 0;

        if (card.getReviewCount() > 0 && !card.getLastReviewDate().empty()) {
            hasReviewed = true;

            if (card.getLastReviewDate() > mostRecentDate) {
                mostRecentDate = card.getLastReviewDate();
                mostRecentCard = card;
            }

            if (card.getLastReviewDate() < oldestDate) {
                oldestDate = card.getLastReviewDate();
                oldestCard = card;
            }
        }
    }

    QString lastReviewDate = "never reviewed";
    if (hasReviewed && !mostRecentDate.empty()) {
        lastReviewDate = QString("last review: %1").arg(QString::fromStdString(mostRecentDate));
    }

    //review label text
    const QString review = QString(
        "%1\n"
        "most recently reviewed: \"%2\"\n"
        "oldest reviewed: \"%3\"\n\n"

        "total reviews: %4\n"
        "average reviews: %5\n"
        "highest reviews: %6\n\n"

        "level: %7\n"
        "hint: %8"
    ).arg(lastReviewDate, hasReviewed ? QString::fromStdString(mostRecentCard.getWord()) : "none", hasReviewed ? QString::fromStdString(oldestCard.getWord()) : "none")
     .arg(totalReviews)
     .arg(!cards.empty() ? QString::number(static_cast<double>(totalReviews) / static_cast<double>(cards.size()), 'f', 1) : "0")
     .arg(maxReviews)
     .arg(totalReviews > 100 ? "advanced" : totalReviews > 50 ? "beginner" : "intermediate")
     .arg(neverReviewed > cards.size() / 3 ? "focus on studying" : neverReviewed < cards.size() / 10 ? "add some cards" : "maintain review consistency");

    //set labels
    insightLabel_->setText(review);
    instructionLabel_->setText("see your review patterns and level");
}

void Insights::showInterval(const std::vector<Card>& cards) const {
    std::vector<Card> adultCards;    // >= 30 days
    std::vector<Card> adolescentCards; // 7-29 days
    std::vector<Card> childCards;    // < 7 days

    int reviewedCards = 0;

    std::vector<int> intervals;

    int maxInterval = 0;

    for (const auto& card : cards) {
        if (card.getReviewCount() > 0) {

            reviewedCards++;
            intervals.push_back(card.getInterval());
            maxInterval = std::max(maxInterval, card.getInterval());

            if (card.getInterval() >= 30) {
                adultCards.push_back(card);
            } else if (card.getInterval() >= 7) {
                adolescentCards.push_back(card);
            } else {
                childCards.push_back(card);
            }
        }
    }

    const double avgInterval = intervals.empty() ? 0 :
    std::accumulate(intervals.begin(), intervals.end(), 0.0) / static_cast<double>(intervals.size());

    //interval label text
    const QString interval = QString(
       "adults: %1\n"
       "adolescents: %2\n"
       "children: %3\n\n"

       "average interval: %4\n"
       "highest interval: %5"
    ).arg(adultCards.size())
     .arg(adolescentCards.size())
     .arg(childCards.size())
     .arg(QString::number(avgInterval, 'f', 1))
     .arg(maxInterval);

    //set labels
    insightLabel_->setText(interval);
    instructionLabel_->setText("check out your cards' demographics");
}

void Insights::showQuality(const std::vector<Card>& cards) const {
    std::vector<Card> easyCards; // some reviews, high ef
    std::vector<Card> strugglingCards; // high reviews, high interval
    std::vector<Card> problemCards; // high reviews, low ef

    int totalReviewed = 0;
    double totalEF = 0;

    for (const auto& card : cards) {
        if (card.getReviewCount() > 0) {
            totalReviewed++;
            totalEF += card.getEasinessFactor();

            //quick
            if (card.getReviewCount() >= 3 && card.getEasinessFactor() > 2.7) {
                easyCards.push_back(card);
            }

            //struggling
            if (card.getReviewCount() >= 8 && card.getInterval() < 14) {
                strugglingCards.push_back(card);
            }

            //problem
            if (card.getReviewCount() >= 5 && card.getEasinessFactor() < 2.2) {
                problemCards.push_back(card);
            }
        }
    }

    const double avgEF = totalReviewed > 0 ? totalEF / totalReviewed : 2.5;

    //most problematic card
    QString worstCard = "none found";
    if (!problemCards.empty()) {
        const auto worst = *std::ranges::min_element(problemCards,
            [](const Card& a, const Card& b) { return a.getEasinessFactor() < b.getEasinessFactor(); });
        worstCard = QString::fromStdString(worst.getWord());
    }

    //quality label text
    const QString quality = QString(
        "average easiness factor (ef): %1\n\n"

        "easy (some reviews, high ef): %4\n"
        "struggling (high reviews, low interval): %5\n"
        "problematic (high reviews, low ef): %2\n"
        "most problematic: \"%3\"\n\n"

        "recommendation: %6"
    ).arg(QString::number(avgEF, 'f', 2))
     .arg(problemCards.size()).arg(worstCard)
     .arg(easyCards.size())
     .arg(strugglingCards.size())
     .arg(static_cast<double>(problemCards.size()) > 0.1*totalReviewed ? "consider improving some cards" :
          easyCards.size() > strugglingCards.size() ? "great quality of recall" :
          "try mnemonics");

    //set labels
    insightLabel_->setText(quality);
    instructionLabel_->setText("track learning effectiveness");
}

void Insights::showEasiness(const std::vector<Card>& cards) const {
    int easyCards = 0;
    int hardCards = 0;

    int quickCards = 0; //cards with high ef after few reviews
    int slowLearners = 0; //cards with low ef after many reviews

    std::vector<double> easinessFactors;
    double totalEFGrowth = 0;

    int totalReviews = 0;
    int reviewedCards = 0;

    for (const auto& card : cards) {
        if (card.getReviewCount() > 0) {

            //easy cards
            if (card.getEasinessFactor() > 2.8) {
                easyCards++;
            }
            //hard cards
            else if (card.getEasinessFactor() < 2.2) {
                hardCards++;
            }

            reviewedCards++;
            totalReviews += card.getReviewCount();
            easinessFactors.push_back(card.getEasinessFactor());
            totalEFGrowth += card.getEasinessFactor() - 2.5;

            //quick to learn cards
            if (card.getReviewCount() < 5 && card.getEasinessFactor() > 2.8) {
                quickCards++;
            }

            //slow to learn cards
            if (card.getReviewCount() > 10 && card.getEasinessFactor() < 2.3) {
                slowLearners++;
            }
        }
    }

    const double avgEFGrowth = reviewedCards > 0 ? totalEFGrowth / reviewedCards : 0;

    //learning speed assessment
    QString speedAssessment;
    if (avgEFGrowth > 0.3) {
        speedAssessment = "shinkansen";
    } else if (avgEFGrowth > 0.1) {
        speedAssessment = "tgv";
    } else if (avgEFGrowth > -0.1) {
        speedAssessment = "intercity";
    } else {
        speedAssessment = "polregio";
    }

    const double avgEF = easinessFactors.empty() ? 2.5 :
        std::accumulate(easinessFactors.begin(), easinessFactors.end(), 0.0) / static_cast<double>(easinessFactors.size());

    //easiness label text
    const QString easiness = QString(
        "easy : %1 cards\n"
        "hard : %2 cards\n"
        "success rate: %3% \n\n"

        "average ef: %4\n"
        "average ef growth: %7\n\n"

        "shinkansens: %5\n"
        "polregios: %6\n"
        "speed assessment: %8"
    ).arg(easyCards).arg(hardCards)
     .arg(reviewedCards > 0 ? QString::number((static_cast<double>(easyCards) / reviewedCards) * 100, 'f', 1) : "0.0", QString::number(avgEF, 'f', 2))
    .arg(quickCards)
     .arg(slowLearners)
     .arg(QString::number(avgEFGrowth, 'f', 2), speedAssessment);

    //set labels
    insightLabel_->setText(easiness);
    instructionLabel_->setText("compare yourself to trains");
}

void Insights::showVocabulary(const std::vector<Card>& cards) const {
    std::vector<int> wordLengths;
    std::map<int, std::vector<double>> lengthToEF; // <length ; ef>  map

    int totalChars = 0;

    int shortWords = 0; // <= 6 chars
    int mediumWords = 0; // 7-10 chars
    int longWords = 0; // > 10 chars

    for (const auto& card : cards) {
        int wordLen = static_cast<int>(card.getWord().length());
        wordLengths.push_back(wordLen);
        totalChars += wordLen;

        if (wordLen <= 6) shortWords++;
        else if (wordLen <= 10) mediumWords++;
        else longWords++;

        //ef by word length for reviewed cards
        if (card.getReviewCount() > 0) {
            lengthToEF[wordLen].push_back(card.getEasinessFactor());
        }
    }

    const double avgWordLength = static_cast<double>(totalChars) / static_cast<double>(cards.size());

    //correlation between length and difficulty
    QString correlation = "not enough data";
    if (!lengthToEF.empty()) {
        double shortAvgEF = 0, longAvgEF = 0;
        int shortCount = 0, longCount = 0;

        for (const auto& [length, efVector] : lengthToEF) {
            const double avgEF = std::accumulate(efVector.begin(), efVector.end(), 0.0) / static_cast<double>(efVector.size());

            if (length <= 6) {
                shortAvgEF += avgEF;
                shortCount++;
            } else if (length > 10) {
                longAvgEF += avgEF;
                longCount++;
            }
        }

        if (shortCount > 0 && longCount > 0) {
            shortAvgEF /= shortCount;
            longAvgEF /= longCount;

            if (shortAvgEF > longAvgEF + 0.1) {
                correlation = "short words are easier to learn";
            } else if (longAvgEF > shortAvgEF + 0.1) {
                correlation = "long words are easier to learn";
            } else {
                correlation = "no strong length-difficulty correlation";
            }
        }
    }

    //vocabulary label text
    const QString vocabulary = QString(
        "average length: %1 characters\n"
        "short: %2 words\n"
        "medium: %3 words\n"
        "long: %4 words\n\n"

        "correlation: %5\n\n"

        "total characters: %6\n"
        "longest word: %7 chars\n"
        "shortest word: %8 chars"
    ).arg(QString::number(avgWordLength, 'f', 1))
     .arg(shortWords).arg(mediumWords).arg(longWords)
     .arg(correlation)
     .arg(totalChars)
     .arg(*std::ranges::max_element(wordLengths))
     .arg(*std::ranges::min_element(wordLengths));

    //set labels
    insightLabel_->setText(vocabulary);
    instructionLabel_->setText("this tab is useful only for vocabulary decks");
}

//style methods
QString Insights::getButtonStyle() {
    return "QPushButton { "
           "color: white; "
           "border: 1px solid white; "
           "font-size: 20px; "
           "font-weight: bold; "
           "border-radius: 0px; "
           "text-align: center; "
           "} "
           "QPushButton:hover { "
           "background-color: #9900bb; "
           "} "
           "QPushButton:pressed { "
           "background-color: #0000ff; "
           "}"
            "QPushButton:disabled {"
            "  background-color: #0000ff;"
            "}";
}

QString Insights::getDisplayStyle() {
    return "QFrame { "
           "border: 1px solid white; "
           "border-radius: 0px; "
           "}";
}

QString Insights::getInsightLabelStyle() {
    return "QLabel { "
           "color: white; "
           "font-size: 22px; "
           "font-weight: bold; "
           "border: none; "
           "}";
}