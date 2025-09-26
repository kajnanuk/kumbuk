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

#include <QApplication>
#include <QMainWindow>
#include <QFont>
#include "Window.h"

int main(int argc, char *argv[]) {


    QApplication app(argc, argv);

    //font
    QFont monoFont("Courier New");
    monoFont.setStyleHint(QFont::Monospace);
    monoFont.setPointSize(12);
    QApplication::setFont(monoFont);

    //app properties
    QApplication::setApplicationName("kumbuk");
    QApplication::setApplicationVersion("0.6");
    QApplication::setOrganizationName("kajnanuk");

    Window window;
    window.show();

    return QApplication::exec();
}