#include "appwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QFile>

#include "boxqinput.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    QApplication a(argc, argv);

    QFile file;
    QString qss;
    file.setFileName(":/qss_black.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    QTranslator qtTran;
    qtTran.load(":/qt_zh_CN.qm");
    a.installTranslator(&qtTran);

    QTranslator qtBase;
    qtBase.load(":/qtbase_zh_CN.qm");
    a.installTranslator(&qtBase);

    AppWindow w;
    w.show();

#ifdef __arm__
    BoxQInput::instance()->hide();
#endif

    return a.exec();
}
