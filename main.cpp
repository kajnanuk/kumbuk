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