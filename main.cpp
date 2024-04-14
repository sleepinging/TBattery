#include "widget.h"
#include <QApplication>
#include <QtDebug>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "batteryevent.h"
#include "config.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    QCommandLineOption only_tray(QStringList() << "t" << "only-tray",u8"仅显示托盘图标");
    parser.addOption(only_tray);
    parser.process(app);

    Config::Init("config.ini");
    Widget w;
    if(!parser.isSet(only_tray)){
        w.show();
    }
    app.installNativeEventFilter(w.btevt_);
    return app.exec();
}
