#ifndef WINDOW_H
#define WINDOW_H
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


#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QFrame>
#include <QMenuBar>
#include <QTimer>
#include <QInputDialog>
#include <QApplication>
#include <QDir>

#include "Deck.h"
#include "Scheduler.h"

class Add;
class Practice;
class Insights;

class Window : public QMainWindow {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window() override;

private slots:
    //slots for mode widgets
    void showAddMode() const;
    void showPracticeMode() const;
    void showInsightsMode() const;

    //deck button actions
    void renameDeck();
    void newDeck();
    void openDeck();
    void saveDeck();
    void saveAsDeck();
    void aboutApp();

private:
    //ui setup
    void setupUI();
    void setupDeckButton();
    void setupNavigationBar();
    void createModeWidgets();
    void recreateModeWidgets();
    //ui segmenty
    QWidget *centralWidget_;
    QVBoxLayout *mainLayout_;

    //deck button
    QPushButton *deckButton_;
    QFrame *deckActionsFrame_;
    QVBoxLayout *deckActionsLayout_;
    //deck button state
    bool deckActionsVisible_;
    //deck actions
    QPushButton *renameDeckButton_{nullptr};
    QPushButton *newDeckButton_{nullptr};
    QPushButton *openDeckButton_{nullptr};
    QPushButton *saveDeckButton_{nullptr};
    QPushButton *saveAsDeckButton_{nullptr};
    QPushButton *aboutButton_{nullptr};
    QPushButton *closeButton_{nullptr};
    //deck message timer
    QTimer* messageTimer_{nullptr};

    //main navigation
    QHBoxLayout *navigationLayout_;
    QFrame *navigationFrame_;
    QPushButton *addButton_;
    QPushButton *practiceButton_;
    QPushButton *insightsButton_;

    //mode widgets
    QStackedWidget *stackedWidget_;
    Add *addWidget_;
    Practice *practiceWidget_;
    Insights *insightsWidget_;

    //data
    Deck currentDeck_;
    Scheduler scheduler_;
    QString currentFilePath_;

    //button style
    QString buttonStyle1_;
    void initialiseButtonStyle1();

    //helpers
    void updateNavigationState() const;
    void setCurrentDeck(const Deck &deck, const QString &filePath = "");
    //deck button helpers
    void updateDeckButton() const;
    void toggleDeckActions();
    bool saveCurrentDeck();
    bool loadDeckFromFile(const QString &filePath);
    void showDeckMessage(const QString &msg, int timeoutMs) const;



};

#endif // WINDOW_H
