#ifndef APPWETHER_H
#define APPWETHER_H

#include <QUrl>
#include <QMovie>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QLayout>
#include <QDateTime>
#include <QMainWindow>
#include <QTextBrowser>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QElapsedTimer>
#include <QCoreApplication>

#include "appscreen.h"
#include "qjson/QJsonArray.h"
#include "qjson/QJsonObject.h"
#include "qjson/QJsonDocument.h"

class AppWether : public QWidget
{
    Q_OBJECT
public:
    explicit AppWether(QWidget *parent = 0);

signals:

public slots:
    void sendFreq(int freq);
private slots:
    void initUI();
    void initLayout();
    void initWether();
    void initNetwork();
    void updateWether();
    virtual void showEvent(QShowEvent *e);
    void wait(int ms);
protected slots:
    void replayWether(QNetworkReply *reply);
    void replayIcblog(QNetworkReply *reply);
private:
    QVBoxLayout *layout;
    QTextBrowser *techo;
    QTextBrowser *guest;
    QNetworkAccessManager *wether;
    QNetworkAccessManager *icblog;
    QList<QLabel*> wethershow;
    AppScreen pwm;
    QTimer *timer;
    int tt;
};

#endif // APPWETHER_H
