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


#include "../include/Add.h"
#include "../include/Deck.h"
#include "../include/Card.h"

Add::Add(Deck* deck, Scheduler* scheduler, QWidget* parent)
    : QWidget(parent), deck_(deck), scheduler_(scheduler), networkManager_(nullptr),
      currentCardIndex_(0), showingFront_(true), isEditing_(false) {
    setupUI();
    loadCards();

    networkManager_ = new QNetworkAccessManager(this);
    connect(networkManager_, &QNetworkAccessManager::finished,
            this, &Add::onWikipediaResponse);
    connect(networkManager_, &QNetworkAccessManager::sslErrors,
            this, &Add::onSslErrors);
}

void Add::onSslErrors(QNetworkReply* reply, const QList<QSslError>& errors) const {
    qDebug() << "ssl errors occurred";
    for (const QSslError& error : errors) {
        qDebug() << "ssl error: " << error.errorString();
    }

    //error message
    instructionLabel_->setText("wikipedia lookup failed - enter definition manually");

    lookupProgress_->setVisible(false);
    buttons_[5]->setEnabled(true);

    reply->abort();
}


void Add::setupUI() {
    //main layout
    mainLayout_ = new QHBoxLayout(this);
    mainLayout_->setContentsMargins(0, 0, 0, 0);
    mainLayout_->setSpacing(2);

    setupDisplayArea();
    setupButtons();
}

void Add::setupDisplayArea() {
    // display frame
    displayFrame_ = new QFrame();
    displayFrame_->setFrameStyle(QFrame::Box);
    displayFrame_->setStyleSheet(getDisplayStyle());
    displayFrame_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // display layout
    displayLayout_ = new QVBoxLayout(displayFrame_);
    displayLayout_->setAlignment(Qt::AlignCenter);
    displayLayout_->setContentsMargins(10, 10, 10, 10);
    displayLayout_->setSpacing(2);

    // card label
    cardLabel_ = new QLabel("select language and add cards");
    cardLabel_->setAlignment(Qt::AlignCenter);
    cardLabel_->setStyleSheet(getCardStyle());
    cardLabel_->setWordWrap(true);
    cardLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //edit fields
    wordEdit_ = new QLineEdit();
    wordEdit_->setPlaceholderText("front side");
    wordEdit_->setStyleSheet("font-size: 20px; font-weight: bold; border: 1px solid white; padding: 10px; margin: 0px;");
    wordEdit_->hide();

    meaningEdit_ = new QTextEdit();
    meaningEdit_->setPlaceholderText("reverse side");
    meaningEdit_->setStyleSheet("font-size: 20px; font-weight: bold; border: 1px solid white; padding: 10px; margin: 0px;");
    meaningEdit_->hide();

    //progress bar (wikipedia lookup)
    lookupProgress_ = new QProgressBar();
    lookupProgress_->setStyleSheet(
        "QProgressBar {"
        " border: 1px solid white;"
        " border-radius: 0px; text-align: center; color: white;"
        "}"
        "QProgressBar::chunk {"
        " background-color: #9900bb; border-radius: 0px;"
        "}"
    );
    lookupProgress_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    lookupProgress_->setVisible(false);

    //intruction label
    instructionLabel_ = new QLabel("select language and add cards");
    instructionLabel_->setAlignment(Qt::AlignCenter);
    instructionLabel_->setStyleSheet(
        "QLabel {"
        " color: #888888;"
        " font-size: 20px;"
        " border: 1px solid #888888;"
        " padding: 10 px;"
        " font-weight: bold;"
        "}"
    );

    displayLayout_->addWidget(cardLabel_);
    displayLayout_->addWidget(wordEdit_);
    displayLayout_->addWidget(meaningEdit_);
    //language selector button
    setupLanguageSelector();
    displayLayout_->addWidget(lookupProgress_);
    displayLayout_->addWidget(instructionLabel_);

    mainLayout_->addWidget(displayFrame_, 3); //0.75 of space
}

void Add::setupButtons() {
    //button frame
    buttonsFrame_ = new QFrame();
    buttonsFrame_->setFrameStyle(QFrame::Box);
    buttonsFrame_->setStyleSheet(
        "QFrame {"
        " border: 1px solid white;"
        " border-radius: 0px;"
        "}"
    );
    buttonsFrame_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //layout
    buttonsLayout_ = new QVBoxLayout(buttonsFrame_);
    buttonsLayout_->setContentsMargins(10, 10, 10, 10);
    buttonsLayout_->setSpacing(2);

    //button labels
    const QStringList buttonLabels = {
        "back",
        "forward",
        "delete",
        "add",
        "edit",
        "wikipedia"
    };

    //create and style buttons
    for (int i = 0; i < 6; i++) {
        buttons_[i] = new QPushButton(buttonLabels[i]);
        buttons_[i]->setStyleSheet(getButtonStyle());
        buttons_[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        buttonsLayout_->addWidget(buttons_[i]);
    }

    //connect buttons
    connect(buttons_[0], &QPushButton::clicked, this, &Add::goBack);
    connect(buttons_[1], &QPushButton::clicked, this, &Add::goForward);
    connect(buttons_[2], &QPushButton::clicked, this, &Add::deleteCard);
    connect(buttons_[3], &QPushButton::clicked, this, &Add::createNewCard);
    connect(buttons_[4], &QPushButton::clicked, this, &Add::editCard);
    connect(buttons_[5], &QPushButton::clicked, this, &Add::lookupWikipedia);

    mainLayout_->addWidget(buttonsFrame_, 1); // 0.25 of space
}

void Add::setupLanguageSelector() {
    languageCombo_ = new QComboBox();
    languageCombo_->setStyleSheet(
        "QComboBox {"
        " font-size: 20px; font-weight: bold; padding: 10px; color: #888888;"
        " border: 1px solid #888888;"
        " border-radius: 0px;"
        "}"
        "QComboBox:hover { background-color: #9900bb; color: white; border: 1px solid white; }"
        "QComboBox QAbstractItemView {"
        " color: #888888;"
        " border: 1px solid #888888;"
        " selection-background-color: #9900bb;"
        "}"
    );

    // languages for the selector
    languageCombo_->addItem("english", "en");
    languageCombo_->addItem("spanish (español)", "es");
    languageCombo_->addItem("french (français)", "fr");
    languageCombo_->addItem("german (Deutsch)", "de");
    languageCombo_->addItem("italian (italiano)", "it");
    languageCombo_->addItem("portuguese (português)", "pt");
    //languageCombo_->addItem("Russian (русский)", "ru"); (bojkot hehe)
    languageCombo_->addItem("chinese (中文)", "zh");
    languageCombo_->addItem("japanese (日本語)", "ja");
    languageCombo_->addItem("korean (한국어)", "ko");
    languageCombo_->addItem("polish (polski)", "pl");

    const auto langLayout = new QHBoxLayout();
    langLayout->addWidget(languageCombo_);
    displayLayout_->addLayout(langLayout);
}

void Add::loadCards() {
    if (deck_) {
        cards_ = deck_->getAllCards();
        if (!cards_.empty()) {
            currentCardIndex_ = 0;
            loadCard(currentCardIndex_);
        } else {
            updateDisplay();
        }
    }
}

void Add::loadCard(const int index) {
    if (index >= 0 && index < cards_.size()) {
        currentCardIndex_ = index;
        showingFront_ = true;
        isEditing_ = false;
        switchToViewMode();
        updateDisplay();
    }
}

//actions
void Add::goBack() {
    if (currentCardIndex_ > 0) {
        if (isEditing_) saveCurrentCard();
        loadCard(currentCardIndex_ - 1);
    }
}

void Add::goForward() {
    if (currentCardIndex_ < cards_.size() - 1) {
        if (isEditing_) saveCurrentCard();
        loadCard(currentCardIndex_ + 1);
    }
}

void Add::deleteCard() {
    if (cards_.empty()) return;

    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "delete card",
        "delete this card?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        deck_->removeCard(currentCardIndex_);
        cards_.erase(cards_.begin() + currentCardIndex_);

        if (cards_.empty()) {
            currentCardIndex_ = 0;
            updateDisplay();
        } else {
            if (currentCardIndex_ >= cards_.size()) {
                currentCardIndex_ = static_cast<int>(cards_.size()) - 1;
            }
            loadCard(currentCardIndex_);
        }
    }
}

void Add::createNewCard() {
    //save currently edited card before adding new
    if (isEditing_) saveCurrentCard();

    //switch to edit mode
    isEditing_ = true;
    updateDisplay();
    wordEdit_->clear();
    meaningEdit_->clear();
    currentCardIndex_ = static_cast<int>(cards_.size());
    switchToEditMode();
    wordEdit_->setFocus();

    //instruction
    instructionLabel_->setText("adding card - enter front, lookup wikipedia");
}

void Add::editCard() {
    if (isEditing_) {
        saveCurrentCard();
    } else {
        //if in view mode, switch to edit

        if (cards_.empty()) {
            createNewCard();
            return;
        }

        isEditing_ = true;
        const Card& card = cards_[currentCardIndex_];

        // load data
        wordEdit_->setText(QString::fromStdString(card.getWord()));
        meaningEdit_->setPlainText(QString::fromStdString(card.getMeaning()));

        switchToEditMode();
        instructionLabel_->setText("editing card - modify and save");
    }
}

void Add::saveCurrentCard() {
    if (!isEditing_) return;

    const QString word = wordEdit_->text().trimmed();
    const QString meaning = meaningEdit_->toPlainText().trimmed();

    if (word.isEmpty() || meaning.isEmpty()) {
        QMessageBox::warning(this, "invalid card",
                           "both word and meaning must be filled in");
        return;
    }

    // create card with new content
    const Card newCard(word.toStdString(), meaning.toStdString());

    // adding card vs editing card
    if (currentCardIndex_ < cards_.size()) {
        // editing card:
        deck_->assignCard(currentCardIndex_, newCard);  //update in actual deck
        cards_[currentCardIndex_] = newCard;  // update in vector
        instructionLabel_->setText("card updated successfully");
    } else {
        // adding card:
        deck_->addCard(newCard);
        cards_.push_back(newCard);
        currentCardIndex_ = static_cast<int>(cards_.size()) - 1;
        instructionLabel_->setText("card saved successfully");
    }

    // back to view mode
    isEditing_ = false;
    showingFront_ = true;
    switchToViewMode();
    updateDisplay();
}

//lookup

void Add::lookupWikipedia() {
    const QString word = wordEdit_->text().trimmed();
    if (word.isEmpty()) {
        if (!isEditing_) {
            createNewCard();
            return;
        }
        QMessageBox::information(this, "wikipedia lookup",
                               "enter something to lookup on wikipedia");
        return;
    }

    currentWord_ = word;
    currentLanguage_ = languageCombo_->currentData().toString();

    //show progress bar
    instructionLabel_->setText("searching wikipedia");
    lookupProgress_->setVisible(true);
    lookupProgress_->setRange(0, 0); //undetermined status
    buttons_[5]->setEnabled(false); //lookup button

    //do the lookup
    doWikipediaLookup(word, currentLanguage_);
}

void Add::doWikipediaLookup(const QString& word, const QString& language) const {
    const QString url = QString("https://%1.wikipedia.org/w/api.php?"
                         "action=query&format=json&prop=extracts&exintro=1&explaintext=1&"
                         "titles=%2&origin=*").arg(language, word);

    QNetworkRequest request{QUrl(url)};
    request.setRawHeader("User-Agent", "kumbukFlashcardsApp/0.5");
    request.setTransferTimeout(15000); // 15 sek

    networkManager_->get(request);
}

void Add::onWikipediaResponse(QNetworkReply* reply) const {
    lookupProgress_->setVisible(false);
    buttons_[5]->setEnabled(true);

    //if got error
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "wikipedia api error:" << reply->errorString();
        instructionLabel_->setText("wikipedia lookup error - enter reverse manually");
        meaningEdit_->setPlainText(QString("wikipedia lookup failed for \"%1\"\n\n"
                                          "you can:\n"
                                          "- check internet connection\n"
                                          "- check spelling\n"
                                          "- try a different language\n"
                                          "- enter the reverse manually").arg(currentWord_));
        reply->deleteLater();
        return;
    }

    const QByteArray data = reply->readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonObject query = obj["query"].toObject();
    QJsonObject pages = query["pages"].toObject();

    bool found = false;
    for (auto it = pages.begin(); it != pages.end(); ++it) {
        QJsonObject page = it.value().toObject();

        if (QString extract = page["extract"].toString(); !extract.isEmpty() && !extract.contains("may refer to")) {
            const QString cleanedExtract = extract.left(800) + (extract.length() > 800 ? "..." : "");
            QString langCode = currentLanguage_.toUpper();
            const QString result = QString("%2").arg(cleanedExtract);

            meaningEdit_->setPlainText(result);
            instructionLabel_->setText("wikipedia definition found - review, edit, save");
            found = true;
            break;
        }
    }

    if (!found) {
        instructionLabel_->setText("no wikipedia article found - enter reverse manually");
        meaningEdit_->setPlainText(QString("wikipedia lookup failed for \"%1\"\n\n"
                                          "you can:\n"
                                          "- check internet connection\n"
                                          "- check spelling\n"
                                          "- try a different language\n"
                                          "- enter the reverse manually").arg(currentWord_));
    }

    reply->deleteLater();
}

QString Add::cleanHtml(const QString& html) {
    QString clean = html;
    clean.remove(QRegularExpression("<[^>]*>"));
    clean = clean.trimmed();
    return clean;
}

//other

void Add::switchToEditMode() const {
    cardLabel_->hide();
    wordEdit_->show();
    meaningEdit_->show();
    buttons_[4]->setText("save");
}

void Add::switchToViewMode() const {
    cardLabel_->show();
    wordEdit_->hide();
    meaningEdit_->hide();
    lookupProgress_->setVisible(false);
    buttons_[4]->setText("edit");
}

void Add::updateDisplay() const {
    if (cards_.empty()) {
        cardLabel_->setText("empty deck");
        instructionLabel_->setText("empty deck - select language and add cards");

        // disable navigation buttons
        buttons_[0]->setEnabled(false); // back
        buttons_[1]->setEnabled(false); // forward
        buttons_[2]->setEnabled(false); // delete
        buttons_[4]->setEnabled(isEditing_); // edit
        return;
    }

    //enable buttons
    buttons_[0]->setEnabled(currentCardIndex_ > 0 && !isEditing_);
    buttons_[1]->setEnabled(currentCardIndex_ < cards_.size() - 1 && !isEditing_);
    buttons_[2]->setEnabled(true);
    buttons_[4]->setEnabled(true);

    if (!isEditing_) {
        const Card& card = cards_[currentCardIndex_];
        if (showingFront_) {
            cardLabel_->setText(QString::fromStdString(card.getWord()));
            instructionLabel_->setText("showing front side");
        } else {
            const QString combined = QString::fromStdString(card.getWord()) +
                              "\n\n-----\n\n" + QString::fromStdString(card.getMeaning());
            cardLabel_->setText(combined);
            instructionLabel_->setText("showing both sides");
        }

        //font style
        const QString text = cardLabel_->text();
        cardLabel_->setStyleSheet(getResponsiveCardStyle(text.length()));
    }
}

void Add::mousePressEvent(QMouseEvent *event) {
    //show meaning when right-clicked
    if (event->button() == Qt::RightButton && !isEditing_) {
        showingFront_ = !showingFront_;
        updateDisplay();
    }

    QWidget::mousePressEvent(event);
}

//style
QString Add::getButtonStyle() {
    return
        "QPushButton {"
        " color: white;"
        " border: 1px solid white;"
        " font-size: 20px;"
        " font-weight: bold;"
        " border-radius: 0px;"
        " text-align: center;"
        "}"
        "QPushButton:hover {"
        " background-color: #9900bb;"
        "}"
        "QPushButton:pressed {"
        " background-color: #0000ff;"
        "}"
        "QPushButton:disabled {"
        " color: #444444;"
        " border-color: #444444;"
        "}";
}

QString Add::getDisplayStyle() {
    return
        "QFrame {"
        " border: 1px solid white;"
        " border-radius: 0px;"
        "}";
}

QString Add::getCardStyle() {
    return
        "QLabel {"
        " color: white;"
        " font-size: 22px;"
        " font-weight: bold;"
        " border: none;"
        "}";
}

QString Add::getResponsiveCardStyle(const int textLength) {
    int fontSize = 22; // default

    //adjust to length
    if (textLength > 200) {
        fontSize = 17;
    } else if (textLength > 100) {
        fontSize = 18;
    } else if (textLength > 50) {
        fontSize = 19;
    }

    return QString(
        "QLabel {"
        " color: white;"
        " font-size: %1px;"
        " font-weight: bold;"
        " border: none;"
        "}"
    ).arg(fontSize);
}