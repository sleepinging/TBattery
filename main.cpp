#include "widget.h"
#include <QApplication>

#include "batteryevent.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    a.installNativeEventFilter(w.btevt_);
    return a.exec();
}
