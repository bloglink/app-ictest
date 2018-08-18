#include "appwether.h"

AppWether::AppWether(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AppWether::sendFreq(int freq)
{
    tt = freq * 1000;
}

void AppWether::initUI()
{
    initLayout();
    initWether();
    initNetwork();
}

void AppWether::initLayout()
{
    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
}

void AppWether::initWether()
{
    QHBoxLayout *wlay = new QHBoxLayout;

    QStringList items;
    items << tr("城市:") << tr("青岛") << tr("风力:") << tr("") << tr("风向:") << tr("")
          << tr("温度:") << tr("") << tr("天气:") << tr("");
    for (int i=0; i < items.size(); i++) {
        QLabel *txt = new QLabel(items.at(i), this);
        wlay->addWidget(txt);
        wethershow.append(txt);
        if (i % 2 == 1)
            wlay->addStretch();
    }
    layout->addLayout(wlay);

    wether = new QNetworkAccessManager(this);
    connect(wether,SIGNAL(finished(QNetworkReply*)),this,SLOT(replayWether(QNetworkReply*)));
    wether->get(QNetworkRequest(QUrl("http://wthrcdn.etouch.cn/weather_mini?citykey=101120201")));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateWether()));
    tt = 60000;
    timer->start(tt);
}

void AppWether::initNetwork()
{
    QHBoxLayout *lay = new QHBoxLayout;
    techo = new QTextBrowser(this);
    lay->addWidget(techo);
    layout->addLayout(lay);

    icblog = new QNetworkAccessManager(this);
    connect(icblog,SIGNAL(finished(QNetworkReply*)),this,SLOT(replayIcblog(QNetworkReply*)));
    icblog->get(QNetworkRequest(QUrl("http://bbs.21ic.com")));
}

void AppWether::updateWether()
{
    QTime from(8, 0, 0);
    QTime stop(20, 0, 0);
    if (QTime::currentTime() > stop || QTime::currentTime() < from)
        pwm.setDuty(0);
    else
        pwm.setDuty(9);
    wether->get(QNetworkRequest(QUrl("http://wthrcdn.etouch.cn/weather_mini?citykey=101120201")));
    icblog->get(QNetworkRequest(QUrl("http://bbs.21ic.com")));
    timer->start(tt);
}

void AppWether::showEvent(QShowEvent *)
{
    updateWether();
}

void AppWether::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void AppWether::replayWether(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();
        QString result = QString::fromLocal8Bit(bytes);
        QJsonObject info = QJsonDocument::fromJson(result.toUtf8()).object();
        QJsonObject data = info.take("data").toObject();
        QJsonValue forecast1 = data.take("forecast");
        QJsonArray forecast2 = forecast1.toArray();
        QJsonObject today = forecast2.at(0).toObject();
        wethershow.at(3)->setText(today.value("fengli").toString().mid(9, 4));
        wethershow.at(5)->setText(today.value("fengxiang").toString());
        wethershow.at(7)->setText(data.value("wendu").toString());
        wethershow.at(9)->setText(today.value("type").toString());
    }
}

void AppWether::replayIcblog(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError) {
        techo->clear();
        QByteArray bytes = reply->readAll();
        QString result = QString::fromLocal8Bit(bytes);
        QStringList tmp = result.split("\n");
        int k = 0;
        int g = 0;
        for (int i=0; i < tmp.size(); i++) {
            if (tmp.at(i).contains(tr("十大热门-技术贴"))) {
                techo->insertHtml(tr("十大热门-技术贴") + "<br><br/>");
                for (int t=0; t < 50; t++) {
                    if (QString(tmp.at(i+t)).contains("<a href=")) {
                        QString str = tmp.at(i+t);
                        QStringList tt = str.split("\"");
                        int x = tt.indexOf(" title=");
                        QString dat = "<a style='font-size:19px;color:cyan;' href=\"0\">";
                        dat += tr("%1&nbsp;&nbsp;").arg(k, 2, 10, QChar('0'));
                        dat += tt.at(x+1);
                        techo->insertHtml(dat + "<br><br/>");
                        k++;
                        if (k >= 10) {
                            techo->insertHtml("<br><br/>");
                            break;
                        }
                    }
                }
            }
            if (tmp.at(i).contains(tr("十大热门-生活贴"))) {
                techo->insertHtml(tr("十大热门-生活贴") + "<br><br/>");
                for (int t=0; t < 50; t++) {
                    if (QString(tmp.at(i+t)).contains("<a href=")) {
                        QString str = tmp.at(i+t);
                        QStringList tt = str.split("\"");
                        int x = tt.indexOf(" title=");
                        QString dat = "<a style='font-size:19px;color:cyan;' href=\"0\">";
                        dat += tr("%1&nbsp;&nbsp;").arg(g, 2, 10, QChar('0'));
                        dat += tt.at(x+1);
                        techo->insertHtml(dat + "<br><br/>");
                        g++;
                        if (g>= 10)
                            break;
                    }
                }
            }
        }
    }
}

