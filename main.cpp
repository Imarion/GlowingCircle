#include "GlowingCircle.h"

#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    MyWindow *window = new MyWindow();
    window->show();

    return a.exec();
}
