#include "appscreen.h"

AppScreen::AppScreen(QObject *parent) : QObject(parent)
{

}

void AppScreen::openfd()
{
    fd = open("/dev/em335x_pwm1", O_RDWR);  // 打开蜂鸣器
    if (fd < 0) {
        qWarning() << "pwm show:" << "open fail";
    }
}

void AppScreen::setDuty(int duty)
{
    QList<int> rates;
    rates << 0 << 1 << 2 << 3 << 5 << 8 << 12 << 17 << 23 << 30 << 100;
    duty = qMin(rates.size()-1, duty);

    struct pwm_config_info conf;

    conf.freq = 20000;
    conf.duty = rates.at(duty);
    conf.polarity = 0;
    conf.count = 0;

    if (duty == 0)
        memset(&conf, 0, sizeof(struct pwm_config_info));

    int ret = write(fd, &conf, sizeof(struct pwm_config_info));
    if (ret < 0) {
        qWarning() << "pwm show:" << "send fail";
    }
}

