#include "SearchUI.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SearchUI w;
    w.setWindowTitle("搜索");
    w.show();

    return a.exec();
}
