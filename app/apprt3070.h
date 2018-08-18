#ifndef APPRT3070_H
#define APPRT3070_H

#include <QFile>
#include <QMovie>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QProcess>
#include <QLineEdit>
#include <QSettings>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QTextBrowser>
#include <QTableWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>

#include "qjson/QJsonArray.h"
#include "qjson/QJsonObject.h"
#include "qjson/QJsonDocument.h"

const QString titleOK = "<p style='font-size:15pt;color:#FFFFFF;' align='left'>%1</p>";
const QString titleNG = "<p style='font-size:15pt;color:#FFFFFF;' align='right'>%1</p>";
const QString strWifi = "<a style='font-size:18px;color:cyan;' align='left' href=\"%1\">%2";

#define MAXNET 5

class AppRT3070 : public QWidget
{
    Q_OBJECT
public:
    explicit AppRT3070(QWidget *parent = 0);

signals:

private slots:
    void initUI();
    void initLayout();
    void initViewBar();
    void initTextBar();
    void initWireLess();
    void initViewItem(int row);
    void initSettings();
    void saveWireLess();
    void clickLink(QString str);
    virtual void showEvent(QShowEvent *e);
    void wait(int t);
private:
    QVBoxLayout *layout;
    QTableWidget *view;
    QList<QLabel*> names;
    QList<QLabel*> texts;
    QList<QLabel*> icons;
    QLineEdit *ssid;
    QLineEdit *pswd;
    QTextBrowser *text;
    QVariantMap wifis;
};

#endif // APPRT3070_H
