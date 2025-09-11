#include <iostream>
#include <chrono>
#include "include/Card.h"
#include "include/Deck.h"
#include <QApplication>
#include <QMainWindow>




int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::cout << "===testtesttest===" << std::endl;

    //test1
    std::cout << "\n1. testing creating cards:" << std::endl;
    Card card1("hello", "A greeting word");
    Card card2("world", "The Earth and all people on it");

    std::cout << card1.getWord() << " - " << card1.getMeaning() << std::endl;
    std::cout << card2.getWord() << " - " << card2.getMeaning() << std::endl;

    //test2
    std::cout << "\n2. testing deck:" << std::endl;
    Deck deck("Test Deck");
    deck.addCard(card1);
    deck.addCard(card2);

    std::cout << "deck: " << deck.getDeckName() << std::endl;
    std::cout << "number of cards: " << deck.getCardCount() << std::endl;

    // test3
    std::cout << "\n3. testing saving:" << std::endl;
    if (deck.saveToFile("../data/test_deck.json")) {
        std::cout << "Saved!" << std::endl;
    }

    //test4
    std::cout << "\n4. testing loading from file:" << std::endl;
    Deck loadedDeck;
    if (loadedDeck.loadFromFile("../data/example_deck.json")) {
        std::cout << "deck: " << loadedDeck.getDeckName() << std::endl;
        std::cout << "number of cards: " << loadedDeck.getCardCount() << std::endl;

        //print all cards
        for (size_t i = 0; i < loadedDeck.getCardCount(); ++i) {
            const Card& card = loadedDeck.getCard(i);
            std::cout << "  " << card.getWord() << " - " << card.getMeaning() << std::endl;
        }
    }

    std::cout << "\n===testttesttest===" << std::endl;

    //show test window
    QMainWindow window;
    window.setWindowTitle("Kumbuk");
    window.resize(600, 400);
    window.show();

    return app.exec();
}