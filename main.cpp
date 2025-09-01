#include <QApplication>
#include <QMainWindow>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qDebug() << "Starting app...";

    QMainWindow window;
    window.setWindowTitle("Test Window");
    window.resize(600, 400);
    window.show();

    qDebug() << "Window shown.";

    return QApplication::exec();
}