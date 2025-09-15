//
// Created by kajna on 05/09/2025.
//

#include "../include/Insights.h"

Insights::Insights(Deck *deck, Scheduler *scheduler, QWidget *parent)
    : QWidget(parent), deck_(deck), scheduler_(scheduler) {

    auto *layout = new QVBoxLayout(this);
    placeholderLabel_ = new QLabel("insights mode TODO");
    placeholderLabel_->setAlignment(Qt::AlignCenter);
    placeholderLabel_->setStyleSheet("font-size: 20px; color: white; padding: 10px;");

    layout->addWidget(placeholderLabel_);
}
