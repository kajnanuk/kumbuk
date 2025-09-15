//
// Created by kajna on 05/09/2025.
//

#ifndef ADD_H
#define ADD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Deck;
class Scheduler;

class Add : public QWidget {
    Q_OBJECT

public:
    explicit Add(Deck *deck, Scheduler *scheduler, QWidget *parent = nullptr);

    ~Add() override = default;

private:
    Deck *deck_;
    Scheduler *scheduler_;
    QLabel *placeholderLabel_;
};

#endif // ADD_H
