//LCDNumber.h File

#ifndef LCDNUMBER_H
#define LCDNUMBER_H

#include <QLCDNumber>
#include <QTimer>
#include <QTime>
class LCDNumber: public QLCDNumber
{
  Q_OBJECT

  public:
  QTimer* timer;
  QTime*  timeValue;
  QTime*  reference_time;
signals:
  void greencal_call();
  public:
        LCDNumber(QWidget * parentWidget,int minutes=0,int seconds=10)
        {
            timer = new QTimer();
            timeValue = new QTime(0,minutes,seconds);
            reference_time= new QTime(0,0,0);
            this->setParent(parentWidget);
            this->setPalette(Qt::black);
            this->display(timeValue->toString());
            QObject::connect(timer,SIGNAL(timeout()),this,SLOT(setDisplay()));
        };
        ~ LCDNumber(){};

   public slots:
        void setDisplay()
        {
          if(((this->timeValue->minute())==(this->reference_time->minute())) && ((this->timeValue->second())==(this->reference_time->second())))
          {
                this->timer->stop();
                emit greencal_call();
                return;
          }
          this->timeValue->setHMS(0,this->timeValue->addSecs(-1).minute(),this->timeValue->addSecs(-1).second());
          this->display(this->timeValue->toString());
        };
};
#endif
