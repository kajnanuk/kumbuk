#include "../include/Window.h"
#include "../include/Add.h"
#include "../include/Practice.h"
#include "../include/Insights.h"

#include <QApplication>
#include <QDir>

Window::Window(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget_(nullptr)
    , mainLayout_(nullptr)
    , deckButton_(nullptr)
    , deckActionsFrame_(nullptr)
    , deckActionsLayout_(nullptr)
    , navigationLayout_(nullptr)
    , navigationFrame_(nullptr)
    , addButton_(nullptr)
    , practiceButton_(nullptr)
    , insightsButton_(nullptr)
    , stackedWidget_(nullptr)
    , addWidget_(nullptr)
    , practiceWidget_(nullptr)
    , insightsWidget_(nullptr)
    , currentDeck_("unnameded deck")
    , deckActionsVisible_(false)
{
    //ustawiamy styl, przygotowujemy interfejs, tworzymy nowy deck, przechodzimy do ekranu decka
    initialiseButtonStyle1();
    setupUI();
    newDeck();
    toggleDeckActions();
}

Window::~Window() = default;

//ui setup

void Window::setupUI() {
    //window props
    setWindowTitle("kumbuk");
    setMinimumSize(800, 600);
    resize(1200, 900);

    //no menu bar
    menuBar()->hide();
    //no status bar
    setStatusBar(nullptr);

    //central widget
    centralWidget_ = new QWidget();
    setCentralWidget(centralWidget_);
    centralWidget_->setStyleSheet("background-color: #050505;");

    //main layout
    mainLayout_ = new QVBoxLayout(centralWidget_);
    mainLayout_->setContentsMargins(10, 10, 10, 10);
    mainLayout_->setSpacing(2);

    //setup ui segments
    setupDeckButton();
    setupNavigationBar();
    createModeWidgets();

}

void Window::setupDeckButton() {
    //create button and style it
    deckButton_ = new QPushButton();
    deckButton_->setMinimumHeight(80);
    deckButton_->setStyleSheet(buttonStyle1_);

    //status message
    messageTimer_ = new QTimer(this);
    messageTimer_->setSingleShot(true);
    connect(messageTimer_, &QTimer::timeout, this, [this]() {
        updateDeckButton();
    });

    //connect button
    connect(deckButton_, &QPushButton::clicked, this, &Window::toggleDeckActions);

    //deck actions frame
    deckActionsFrame_ = new QFrame();
    deckActionsFrame_->setFrameStyle(QFrame::Box);
    deckActionsFrame_->setStyleSheet(
        "QFrame {"
        "   border: 1px solid white;"
        "   border-radius: 0px;"
        "}"
    );
    deckActionsFrame_->hide();

    //deck actions layout
    deckActionsLayout_ = new QVBoxLayout(deckActionsFrame_);
    deckActionsLayout_->setContentsMargins(10, 10, 10, 10);
    deckActionsLayout_->setSpacing(2);

    //deck actions buttons

    newDeckButton_ = new QPushButton("new deck");
    openDeckButton_ = new QPushButton("open deck");
    saveDeckButton_ = new QPushButton("save deck");
    saveAsDeckButton_ = new QPushButton("save as...");
    aboutButton_ = new QPushButton("about kumbuk");
    closeButton = new QPushButton("close");

    //actions styling
    newDeckButton_->setStyleSheet(buttonStyle1_);
    openDeckButton_->setStyleSheet(buttonStyle1_);
    saveDeckButton_->setStyleSheet(buttonStyle1_);
    saveAsDeckButton_->setStyleSheet(buttonStyle1_);
    aboutButton_->setStyleSheet(buttonStyle1_);
    closeButton->setStyleSheet(buttonStyle1_);
    //actions size policy
    newDeckButton_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    openDeckButton_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    saveDeckButton_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    saveAsDeckButton_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    aboutButton_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //connecting signals
    connect(newDeckButton_, &QPushButton::clicked, this, &Window::newDeck);
    connect(openDeckButton_, &QPushButton::clicked, this, &Window::openDeck);
    connect(saveDeckButton_, &QPushButton::clicked, this, &Window::saveDeck);
    connect(saveAsDeckButton_, &QPushButton::clicked, this, &Window::saveAsDeck);
    connect(aboutButton_, &QPushButton::clicked, this, &Window::aboutApp);
    connect(closeButton, &QPushButton::clicked, this, &Window::toggleDeckActions);

    //add buttons to layout
    deckActionsLayout_->addWidget(newDeckButton_);
    deckActionsLayout_->addWidget(openDeckButton_);
    deckActionsLayout_->addWidget(saveDeckButton_);
    deckActionsLayout_->addWidget(saveAsDeckButton_);
    deckActionsLayout_->addStretch();
    deckActionsLayout_->addWidget(aboutButton_);
    deckActionsLayout_->addStretch();
    deckActionsLayout_->addWidget(closeButton);

    //add to main layout
    mainLayout_->addWidget(deckButton_, 0);
    mainLayout_->addWidget(deckActionsFrame_, 0);
}

void Window::setupNavigationBar() {
    //navigation frame
    navigationFrame_ = new QFrame();
    navigationFrame_->setMinimumHeight(120);
    navigationLayout_ = new QHBoxLayout(navigationFrame_);
    navigationLayout_->setSpacing(2);
    navigationLayout_->setContentsMargins(0, 0, 0, 0);

    //navigation buttons
    addButton_ = new QPushButton("add");
    practiceButton_ = new QPushButton("practice");
    insightsButton_ = new QPushButton("statistics");

    //button style
    addButton_->setStyleSheet(buttonStyle1_);
    practiceButton_->setStyleSheet(buttonStyle1_);
    insightsButton_->setStyleSheet(buttonStyle1_);

    //button size policy
    addButton_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    practiceButton_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    insightsButton_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // checkable buttons
    addButton_->setCheckable(true);
    practiceButton_->setCheckable(true);
    insightsButton_->setCheckable(true);

    //connect buttons
    connect(addButton_, &QPushButton::clicked, this, &Window::showAddMode);
    connect(practiceButton_, &QPushButton::clicked, this, &Window::showPracticeMode);
    connect(insightsButton_, &QPushButton::clicked, this, &Window::showInsightsMode);

    //add buttons to layout
    navigationLayout_->addWidget(addButton_);
    navigationLayout_->addWidget(practiceButton_);
    navigationLayout_->addWidget(insightsButton_);

    //add navigation frame to main layout
    mainLayout_->addWidget(navigationFrame_, 0);
}

void Window::createModeWidgets() {
    //stacked widget for different modes
    stackedWidget_ = new QStackedWidget();

    //mode widgets
    addWidget_ = new Add(&currentDeck_, &scheduler_);
    practiceWidget_ = new Practice(&currentDeck_, &scheduler_);
    insightsWidget_ = new Insights(&currentDeck_, &scheduler_);

    //add widgets to stack
    stackedWidget_->addWidget(addWidget_);      //index 0
    stackedWidget_->addWidget(practiceWidget_);  //index 1
    stackedWidget_->addWidget(insightsWidget_);  //index 2

    //add stacked widget to main layout
    mainLayout_->addWidget(stackedWidget_, 1);
}

// navigation slots
void Window::showAddMode() const {
    if (deckActionsVisible_) {
        const_cast<Window*>(this)->toggleDeckActions();
    }
    //set index
    stackedWidget_->setCurrentIndex(0);
    //checked status
    addButton_->setChecked(true);
    practiceButton_->setChecked(false);
    insightsButton_->setChecked(false);
}

void Window::showPracticeMode() const {
    if (deckActionsVisible_) {
        const_cast<Window*>(this)->toggleDeckActions();
    }
    //set index
    stackedWidget_->setCurrentIndex(1);
    //checked status
    addButton_->setChecked(false);
    practiceButton_->setChecked(true);
    insightsButton_->setChecked(false);
}

void Window::showInsightsMode() const {
    if (deckActionsVisible_) {
        const_cast<Window*>(this)->toggleDeckActions();
    }
    //set index
    stackedWidget_->setCurrentIndex(2);
    //checked status
    addButton_->setChecked(false);
    practiceButton_->setChecked(false);
    insightsButton_->setChecked(true);
}

//deck button and actions

void Window::toggleDeckActions() {
    //just reverse everything
    deckActionsVisible_ = !deckActionsVisible_;

    if (deckActionsVisible_) {
        deckActionsFrame_->show();
        navigationFrame_->hide();
        stackedWidget_->hide();
    } else {
        deckActionsFrame_->hide();
        navigationFrame_->show();
        stackedWidget_->show();
    }
}

void Window::newDeck() {
    Deck newDeck("unnamed deck");
    setCurrentDeck(newDeck);
    toggleDeckActions();
    showAddMode();
}

void Window::openDeck() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "open deck",
        QDir::currentPath() + "/data",
        "JSON Files (*.json);;All Files (*)"
    );

    if (!filePath.isEmpty()) {
        loadDeckFromFile(filePath);
        toggleDeckActions();
    }
}

void Window::saveDeck() {
    if (currentFilePath_.isEmpty()) {
        saveAsDeck();
    } else {
        saveCurrentDeck();
        toggleDeckActions();
    }
}

void Window::saveAsDeck() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "save deck",
        QDir::currentPath() + "/data/" + QString::fromStdString(currentDeck_.getDeckName()) + ".json",
        "JSON Files (*.json);;All Files (*)"
    );

    if (!filePath.isEmpty()) {
        currentFilePath_ = filePath;
        saveCurrentDeck();
        toggleDeckActions();
    }
}

void Window::aboutApp() {
    QMessageBox::about(this, "about kumbuk",
        "kumbuk is\n\n"
        "a flashcards app with spaced repetition.\n\n"
        "Qt6 used.\nnlohmann/json used.\nsm-2 used.\n\n"
        "© kajnanuk 2025");
}

//button style

void Window::initialiseButtonStyle1() {
    buttonStyle1_ =
        "QPushButton {"
        "   color: white;"
        "   border: 1px solid white;"
        "   padding: 10px 20px;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   border-radius: 0px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #9900bb;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #0000ff;"
        "}"
        "QPushButton:checked {"
        "   background-color: #0000ff;"
        "}";
}

//helpers

void Window::updateNavigationState() const {
    //if the deck is empty, allow only add mode
    bool hasCards = currentDeck_.getCardCount() > 0;

    practiceButton_->setEnabled(hasCards);
    insightsButton_->setEnabled(hasCards);
    addButton_->setEnabled(true);

    if (!hasCards && stackedWidget_->currentIndex() != 0) {
        showAddMode();
    }
}

void Window::setCurrentDeck(const Deck &deck, const QString &filePath) {
    currentDeck_ = deck;
    currentFilePath_ = filePath;
    updateDeckButton();
    updateNavigationState();
}

void Window::updateDeckButton() const {
    QString deckInfo = QString("%1")
        .arg(QString::fromStdString(currentDeck_.getDeckName()));
    deckButton_->setText(deckInfo);
}

bool Window::saveCurrentDeck() {
    if (currentFilePath_.isEmpty()) {
        return false;
    }

    if (currentDeck_.saveToFile(currentFilePath_.toStdString())) {
        showDeckMessage("DECK SAVED", 2000);
        return true;
    } else {
        QMessageBox::warning(this, "save error", "could not save deck to file.");
        return false;
    }
}

bool Window::loadDeckFromFile(const QString &filePath) {
    Deck loadedDeck;
    if (loadedDeck.loadFromFile(filePath.toStdString())) {
        setCurrentDeck(loadedDeck, filePath);
        showDeckMessage("DECK LOADED", 2000);
        return true;
    } else {
        QMessageBox::warning(this, "load error", "could not load deck from file.");
        return false;
    }
}

void Window::showDeckMessage(const QString& message, int timeoutInMs = 2000) {
    //stop previous timer
    if (messageTimer_->isActive()) {
        messageTimer_->stop();
    }

    //show message
    deckButton_->setText(message);

    //restart timer to restore normal text
    messageTimer_->start(timeoutInMs);
}