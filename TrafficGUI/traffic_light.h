#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QState>
#include <QStateMachine>
#include <QTimer>
#include <QFinalState>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>

class LightWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool on READ isOn WRITE setOn)
public:
    LightWidget(const QColor &color, QWidget *parent = 0)
        : QWidget(parent), m_color(color), m_on(false) {}

    bool isOn() const
        { return m_on; }
    void setOn(bool on)
    {
        if (on == m_on)
            return;
        m_on = on;
        update();
    }

public slots:
    void turnOff() { setOn(false); }
    void turnOn() { setOn(true); }

protected:
    void paintEvent(QPaintEvent *) override
    {
        if (!m_on)
            return;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(m_color);
        painter.drawEllipse(0, 0, width(), height());
    }

private:
    QColor m_color;
    bool m_on;
};

class TrafficLightWidget : public QWidget
{
public:
    TrafficLightWidget(QWidget *parent = 0)
        : QWidget(parent)
    {
        QVBoxLayout *vbox = new QVBoxLayout(this);
        m_red = new LightWidget(Qt::red);
        vbox->addWidget(m_red);
        m_yellow = new LightWidget(Qt::yellow);
        vbox->addWidget(m_yellow);
        m_green = new LightWidget(Qt::green);
        vbox->addWidget(m_green);
        QPalette pal = palette();
        pal.setColor(QPalette::Background, Qt::black);
        setPalette(pal);
        setAutoFillBackground(true);
    }

    LightWidget *redLight() const
        { return m_red; }
    LightWidget *yellowLight() const
        { return m_yellow; }
    LightWidget *greenLight() const
        { return m_green; }

private:
    LightWidget *m_red;
    LightWidget *m_yellow;
    LightWidget *m_green;
};

QState *createLightState(LightWidget *light, int duration, QState *parent= 0);
QState *createStaticLightState(LightWidget *light, QState *parent= 0);


class TrafficLight : public QWidget
{
public:
    TrafficLight(QWidget *parent = 0)
        : QWidget(parent)
    {
        vbox = new QVBoxLayout(this);
        widget = new TrafficLightWidget();
        vbox->addWidget(widget);
        vbox->setMargin(0);
        machine = new QStateMachine(this);
        machine1= new QStateMachine(this);
    }
    ~TrafficLight()
    {
        delete vbox;
        delete machine;
        delete widget;
    }
    //Initialize state machine states
    void Start();
    //Change to green light
    void ChangetoGreen();
    //Change to red light
    void ChangetoRed();
private:
    QVBoxLayout *vbox;
    QStateMachine *machine;
    QStateMachine *machine1;
    QState *redGoingYellow;
    QState *yellowGoingGreen;
    QState *greenGoingYellow;
    QState *yellowGoingRed;
    QState  *greenStatic;
    QState  *redStatic;
    TrafficLightWidget *widget;
};



