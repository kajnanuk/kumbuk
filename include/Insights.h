//
// Created by kajna on 05/09/2025.
//

#ifndef INSIGHTS_H
#define INSIGHTS_H



#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Deck;
class Scheduler;

class Insights : public QWidget {
    Q_OBJECT

public:
    explicit Insights(Deck *deck, Scheduler *scheduler, QWidget *parent = nullptr);

    ~Insights() override = default;

private:
    Deck *deck_;
    Scheduler *scheduler_;
    QLabel *placeholderLabel_;
};



#endif //INSIGHTS_H
