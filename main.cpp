#include <QApplication>
#include <QMainWindow>
#include <iostream>
#include "Card.h"
#include "Deck.h"
#include "Scheduler.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::cout << "===testtesttest===" << std::endl;

    // test1
    std::cout << "\n1. creating:" << std::endl;
    Card card1("hello", "A greeting word");
    Card card2("world", "The Earth and all people on it");
    Card card3("algorithm", "A step-by-step procedure for calculations");

    Deck deck("Test Deck");
    deck.addCard(card1);
    deck.addCard(card2);
    deck.addCard(card3);

    std::cout << "Created deck with " << deck.getCardCount() << " cards" << std::endl;

    // test2
    std::cout << "\n2. scheduler:" << std::endl;

    //simulate practice
    Card& practiceCard = deck.getCard(0);
    std::cout << "Practicing card: " << practiceCard.getWord() << std::endl;
    std::cout << "Initial state:" << std::endl;
    std::cout << "  EF: " << practiceCard.getEasinessFactor() << std::endl;
    std::cout << "  Interval: " << practiceCard.getInterval() << std::endl;
    std::cout << "  Review count: " << practiceCard.getReviewCount() << std::endl;

    // reviewing simulation
    std::cout << "\n---simulating---" << std::endl;

    //I - incorrect difficult (quality 3)
    Scheduler::updateCardSchedule(practiceCard, Scheduler::Quality::INCORRECT_DIFFICULT);

    //II  -  perfect (quality 5)
    Scheduler::updateCardSchedule(practiceCard, Scheduler::Quality::PERFECT);

    //III - hesitant (quality 4)
    Scheduler::updateCardSchedule(practiceCard, Scheduler::Quality::CORRECT_HESITANT);

    // test3
    std::cout << "\n3. due cards check:" << std::endl;

    std::vector<Card> allCards;
    for (size_t i = 0; i < deck.getCardCount(); ++i) {
        allCards.push_back(deck.getCard(i));
    }

    auto dueCards = Scheduler::getDueCards(allCards);
    std::cout << "due cards: " << dueCards.size() << std::endl;

    auto newCards = Scheduler::getNewCards(allCards, 5);
    std::cout << "new cards: " << newCards.size() << std::endl;

    auto reviewCards = Scheduler::getCardsForReview(allCards);

    std::cout << "cards for review: " <<std::endl;
    for (const auto& i:reviewCards) {
        std::cout<<i.getWord()<<std::endl;
    }

    // test4
    std::cout << "\n4. saving:" << std::endl;
    if (deck.saveToFile("../data/test_deck_with_scheduling.json")) {
        std::cout << "Saved!" << std::endl;
    }

    std::cout << "\n===testtesttest===" << std::endl;

    //show window
    QMainWindow window;
    window.setWindowTitle("kumbuk");
    window.resize(600, 400);
    window.show();

    return QApplication::exec();
}
