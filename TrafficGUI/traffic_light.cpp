#include <traffic_light.h>


QState *createLightState(LightWidget *light, int duration, QState *parent)
{
    QState *lightState = new QState(parent);
    QTimer *timer = new QTimer(lightState);
    timer->setInterval(duration);
    timer->setSingleShot(true);
    QState *timing = new QState(lightState);
    QObject::connect(timing, SIGNAL(entered()), light, SLOT(turnOn()));
    QObject::connect(timing, SIGNAL(entered()), timer, SLOT(start()));
    QObject::connect(timing, SIGNAL(exited()), light, SLOT(turnOff()));
    QFinalState *done = new QFinalState(lightState);
    timing->addTransition(timer, SIGNAL(timeout()), done);
    lightState->setInitialState(timing);
    return lightState;
}

QState *createStaticLightState(LightWidget *light,QState *parent)
{
    QState *lightState = new QState(parent);
    QState *timing = new QState(lightState);
    QObject::connect(timing, SIGNAL(entered()), light, SLOT(turnOn()));
    lightState->setInitialState(timing);
    return lightState;
}

void TrafficLight::Start(){
    this->greenStatic = createStaticLightState(widget->greenLight());
    this->redStatic = createStaticLightState(widget->redLight());
    this->redGoingYellow = createLightState(widget->redLight(), 100);
    this->redGoingYellow->setObjectName("redGoingYellow");
    this->yellowGoingGreen = createLightState(widget->yellowLight(), 1000);
    this->yellowGoingGreen->setObjectName("yellowGoingGreen");
    this->redGoingYellow->addTransition(this->redGoingYellow, SIGNAL(finished()), this->yellowGoingGreen);
    this->greenGoingYellow = createLightState(widget->greenLight(), 100);
    this->greenGoingYellow->setObjectName("greenGoingYellow");
    this->yellowGoingGreen->addTransition(yellowGoingGreen, SIGNAL(finished()), greenStatic);
    this->yellowGoingRed = createLightState(widget->yellowLight(), 1000);
    this->yellowGoingRed->setObjectName("yellowGoingRed");
    this->greenGoingYellow->addTransition(this->greenGoingYellow, SIGNAL(finished()), this->yellowGoingRed);
    this->yellowGoingRed->addTransition(yellowGoingRed, SIGNAL(finished()), redStatic);
    machine->addState(this->redGoingYellow);
    machine->addState(this->yellowGoingGreen);
    machine->addState(this->greenStatic);
    machine->setInitialState(this->redGoingYellow);
    machine1->addState(this->greenGoingYellow);
    machine1->addState(this->yellowGoingRed);
    machine1->addState(this->redStatic);
    machine1->setInitialState(this->greenGoingYellow);
}

void TrafficLight::ChangetoGreen()
{

    machine1->stop();
    machine->start();
}

void TrafficLight::ChangetoRed()
{
    machine->stop();
    machine1->start();
}

