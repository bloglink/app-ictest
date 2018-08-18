#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QUrl>
#include <QMovie>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QLayout>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSettings>

#include "appwether.h"
#include "apprt3070.h"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = 0);
    ~AppWindow();
private slots:
    void initUI();
    void initTitle();
    void initLayout();
    void initWether();
    void initRT3070();
    void initButton();
    void clickButtons();
    void updateTime();
    void initSettings();
    void saveSettings();
    virtual void cloudAntimation();
    void wait(int ms);
    void play();
    virtual void showEvent(QShowEvent *);
private:
    QVBoxLayout *layout;
    QStackedWidget *stack;
    int preindex;
    QLineEdit *time;
    AppWether *wether;
    QLabel *beep;
    QMovie *movie;

};

#endif // APPWINDOW_H
