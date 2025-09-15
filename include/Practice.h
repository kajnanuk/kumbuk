//
// Created by kajna on 05/09/2025.
//

#ifndef PRACTICE_H
#define PRACTICE_H



#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Deck;
class Scheduler;

class Practice : public QWidget {
    Q_OBJECT

public:
    explicit Practice(Deck *deck, Scheduler *scheduler, QWidget *parent = nullptr);

    ~Practice() override = default;

private:
    Deck *deck_;
    Scheduler *scheduler_;
    QLabel *placeholderLabel_;
};



#endif //PRACTICE_H
