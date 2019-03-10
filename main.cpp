#include "widget.h"
#include <QApplication>

#include "batteryevent.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Config::Init("config.ini");
    Widget w;
    w.show();
    a.installNativeEventFilter(w.btevt_);
    return a.exec();
}
