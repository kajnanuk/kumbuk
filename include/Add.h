#ifndef ADD_H
#define ADD_H
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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMouseEvent>
#include <QUrl>
#include <QComboBox>
#include <QProgressBar>
#include <QRegularExpression>
#include <QSslError>
#include <vector>

class Deck;
class Scheduler;
class Card;

class Add final : public QWidget {
    Q_OBJECT

public:

    explicit Add(Deck* deck, Scheduler* scheduler, QWidget* parent = nullptr);
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) const;

private slots:
    //buttons
    void goBack();
    void goForward();
    void deleteCard();
    void createNewCard();
    void editCard();
    void saveCurrentCard();
    //wikipedia lookup
    void lookupWikipedia();
    void onWikipediaResponse(QNetworkReply* reply) const;

    static QString getWikipediaName(const QString &languageCode);

private:
    //data
    Deck* deck_;
    Scheduler* scheduler_;

    //ui
    QHBoxLayout* mainLayout_{};
    QFrame* displayFrame_{};
    QVBoxLayout* displayLayout_{};
    QLabel* cardLabel_{};
    QLabel* instructionLabel_{};
    QLineEdit* wordEdit_{};
    QTextEdit* meaningEdit_{};
    //buttons
    QFrame* buttonsFrame_{};
    QVBoxLayout* buttonsLayout_{};
    QPushButton *buttons_[6]{};

    //language selector
    QComboBox* languageCombo_{};
    QProgressBar* lookupProgress_{};

    // network
    QNetworkAccessManager* networkManager_;
    QString currentWord_;
    QString currentLanguage_;

    //state
    std::vector<Card> cards_;
    int currentCardIndex_;
    bool showingFront_;
    bool isEditing_;

    //ui setup
    void setupUI();
    void setupDisplayArea();
    void setupButtons();
    void setupLanguageSelector();

    //managing cards
    void loadCard(int index);
    void loadCards();

    //mode
    void switchToEditMode() const;
    void switchToViewMode() const;

    //wikipedia lookup
    void doWikipediaLookup(const QString& word, const QString& language) const;

    static QString cleanHtml(const QString& html);

    //style
    static QString getButtonStyle();

    static QString getDisplayStyle();

    static QString getCardStyle();

    static QString getResponsiveCardStyle(int textLength);

    //other
    void updateDisplay() const;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // ADD_H

