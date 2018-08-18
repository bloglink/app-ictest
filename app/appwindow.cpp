#include "appwindow.h"

const QString largeOK = "<p style='font-size:15pt;color:#00FF00;' align='center'>%1</p>";

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    initUI();
}

AppWindow::~AppWindow()
{
}

void AppWindow::initUI()
{
    initTitle();
    initLayout();
    initWether();
    initRT3070();
    initButton();
}

void AppWindow::initTitle()
{
#ifdef __arm__
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    char s_month[5];
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    int month, day, year;

    sscanf((__DATE__), "%s %d %d", s_month, &day, &year);
    month = (strstr(month_names, s_month)-month_names)/3+1;

    QDate dt;
    dt.setDate(year, month, day);
    static const QTime tt = QTime::fromString(__TIME__, "hh:mm:ss");

    QDateTime t(dt, tt);
    QString verNumb = QString("V-0.1-%1").arg(t.toString("yyMMdd-hhmm"));
    this->setWindowTitle(tr("网络信息采集器%1").arg(verNumb));
    qDebug() << verNumb;
}

void AppWindow::initLayout()
{
    layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    stack = new QStackedWidget(this);
    layout->addWidget(stack);

    QFrame *frame = new QFrame(this);
    frame->setLayout(layout);

    this->setCentralWidget(frame);
    this->resize(800, 600);
}

void AppWindow::initWether()
{
    wether = new AppWether(this);
    stack->addWidget(wether);

    beep = new QLabel(this);
    movie = new QMovie(":/04.gif");
    beep->setMovie(movie);
    beep->hide();
}

void AppWindow::initRT3070()
{
    AppRT3070 *app = new AppRT3070(this);
    stack->addWidget(app);
}

void AppWindow::initButton()
{
    preindex = 0;
    QHBoxLayout *blayout = new QHBoxLayout;
    layout->addLayout(blayout);

    QPushButton *btnWether = new QPushButton(tr("网络信息"), this);

    btnWether->setFlat(true);
    btnWether->setCheckable(true);
    btnWether->setFixedSize(97, 44);
    btnWether->setObjectName("wether");
    btnWether->setFocusPolicy(Qt::NoFocus);
    blayout->addWidget(btnWether);
    connect(btnWether, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    QPushButton *btnRT3070 = new QPushButton(tr("网络配置"), this);
    btnRT3070->setFlat(true);
    btnRT3070->setCheckable(true);
    btnRT3070->setFixedSize(97, 44);
    btnRT3070->setObjectName("RT3070");
    btnRT3070->setFocusPolicy(Qt::NoFocus);
    blayout->addWidget(btnRT3070);
    connect(btnRT3070, SIGNAL(clicked(bool)), this, SLOT(clickButtons()));

    blayout->addWidget(new QLabel(tr("刷新时间(s)"), this));
    time = new QLineEdit(this);
//    time->setText("60");
    blayout->addWidget(time);
    connect(time, SIGNAL(editingFinished()), this, SLOT(updateTime()));

    blayout->addStretch();

    QLabel *text = new QLabel(this);
    QString str = largeOK.arg("预祝【21ic第二届设计大赛】完满成功&nbsp;by link 8001");
    text->setText(str);
    blayout->addWidget(text);
}

void AppWindow::clickButtons()
{
    QString name = sender()->objectName();
    stack->setCurrentIndex(name == "wether" ? 0 : 1);
    preindex = (name == "wether") ? 1 : 0;
    cloudAntimation();
    QTimer::singleShot(2000, this, SLOT(play()));
}

void AppWindow::updateTime()
{
    wether->sendFreq(time->text().toInt());
    saveSettings();
    play();
}

void AppWindow::initSettings()
{
    QSettings *set = new QSettings("config.ini", QSettings::IniFormat);
    QString str = set->value("freq", "60").toString();
    time->setText(str);
}

void AppWindow::saveSettings()
{
    QSettings *set = new QSettings("config.ini", QSettings::IniFormat);
    set->setValue("freq", time->text());
}

void AppWindow::cloudAntimation()
{
    QLabel* circle = new QLabel(stack->currentWidget());
    QLabel* line = new QLabel(this);
    line->setObjectName(QString("AntimationLine"));
    line->resize(0, 2);
    line->show();
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    circle->setPixmap(QPixmap::grabWindow(stack->widget(preindex)->winId()));
#else
    circle->setPixmap(stack->widget(preindex)->grab());
#endif
    circle->show();
    circle->resize(stack->currentWidget()->size());
    QPropertyAnimation *animation = new QPropertyAnimation(circle, "geometry");

    animation->setDuration(1000);
    animation->setStartValue(circle->geometry());

    QPropertyAnimation* animation_line = new QPropertyAnimation(line, "size");
    animation_line->setDuration(1000);
    animation_line->setEasingCurve(QEasingCurve::OutCubic);

    animation->setEndValue(QRect(-3, 0, 0, circle->height()));
    line->move(stack->x(), stack->y() - 2);
    animation_line->setStartValue(QSize(0, 2));
    animation_line->setEndValue(QSize(stack->width(), 2));

    animation->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* animation_opacity = new QPropertyAnimation(circle, "windowOpacity");
    animation_opacity->setDuration(1000);
    animation_opacity->setStartValue(1);
    animation_opacity->setEndValue(0);
    animation_opacity->setEasingCurve(QEasingCurve::OutCubic);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;

    connect(group, SIGNAL(finished()), circle, SLOT(hide()));
    connect(group, SIGNAL(finished()), circle, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), line, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), group, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation_opacity, SLOT(deleteLater()));
    connect(group, SIGNAL(finished()), animation_line, SLOT(deleteLater()));
    group->addAnimation(animation);
    group->addAnimation(animation_opacity);
    group->addAnimation(animation_line);
    group->start();
}

void AppWindow::wait(int ms)
{
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms)
        QCoreApplication::processEvents();
}

void AppWindow::play()
{
    beep->show();
    beep->resize(50, 50);
    movie->start();
    for (int i=0; i < 100; i++) {
        beep->move(i*10, 175);
        wait(100);
    }
    beep->hide();
}

void AppWindow::showEvent(QShowEvent *)
{
    initSettings();
}
