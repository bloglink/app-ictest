#ifndef APPSCREEN_H
#define APPSCREEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <QDebug>
#include <QObject>

struct pwm_config_info {
    unsigned int freq;
    unsigned int duty;
    unsigned int polarity;
    unsigned int count;
};

class AppScreen : public QObject
{
    Q_OBJECT
public:
    explicit AppScreen(QObject *parent = 0);

signals:

public slots:
    void openfd();
    void setDuty(int duty);
private:
    int fd;
};

#endif // APPSCREEN_H
