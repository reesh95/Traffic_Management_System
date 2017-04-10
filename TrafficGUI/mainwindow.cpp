#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

struct{
    int count_of_cars;
    int green_lit;
    int green_time;
    int wait_time;
    int masked;
    int prev_count_of_cars;
} para_road[4];


int global_signal_time= 120;

static void Init_params();
//struct road_parameter para_road[4];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Init_params();
    DTMS= true;
    QGridLayout *layout = new QGridLayout(this);
    myPlayerNorth = new Player();
    myPlayerSouth = new Player();
    myPlayerEast = new Player();
    myPlayerWest = new Player();
    widget_North= new TrafficLight(this);
    widget_North->resize(50, 100);
    widget_North->move(720, 390);
    widget_South= new TrafficLight(this);
    widget_South->resize(50, 100);
    widget_South->move(720, 495);
    widget_East= new TrafficLight(this);
    widget_East->resize(50, 100);
    widget_East->move(910, 500);
    widget_West= new TrafficLight(this);
    widget_West->resize(50, 100);
    widget_West->move(510, 500);
    time_North= new LCDNumber(this);
    time_South= new LCDNumber(this,0,0);
    time_East= new LCDNumber(this,0,0);
    time_West= new LCDNumber(this,0,0);
    time_North->resize(75,50);
    time_South->resize(75,50);
    time_East->resize(75,50);
    time_West->resize(75,50);
    time_North->move(710, 335);
    time_South->move(710, 600);
    time_East->move(900, 440);
    time_West->move(500, 440);
    layout->addWidget(widget_North,0,0);
    layout->addWidget(widget_South,0,0);
    layout->addWidget(widget_East,0,0);
    layout->addWidget(widget_West,0,0);
    layout->addWidget(time_West,0,0);
    layout->addWidget(time_East,0,0);
    layout->addWidget(time_North,0,0);
    layout->addWidget(time_South,0,0);

    QObject::connect(myPlayerNorth, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI(QImage)));
    QObject::connect(myPlayerSouth, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI1(QImage)));
    QObject::connect(myPlayerEast, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI2(QImage)));
    QObject::connect(myPlayerWest, SIGNAL(processedImage(QImage)),
                              this, SLOT(updatePlayerUI3(QImage)));
    QObject::connect(time_North, SIGNAL(greencal_call()),
                              this, SLOT(greenTimeCalculator_point()));
    QObject::connect(time_South, SIGNAL(greencal_call()),
                              this, SLOT(greenTimeCalculator_point()));
    QObject::connect(time_East, SIGNAL(greencal_call()),
                              this, SLOT(greenTimeCalculator_point()));
    QObject::connect(time_West, SIGNAL(greencal_call()),
                              this, SLOT(greenTimeCalculator_point()));

    ui->setupUi(this);
    ui->textEdit->setText("Current running algorithm is DTMS");
    QPixmap *pix= new QPixmap("../TrafficGUI/Analog-Devices.jpg");
    QPixmap *pix1= new QPixmap("../TrafficGUI/RVCE_New_Logo.jpg");
    ui->logo->setPixmap(pix->scaled(ui->logo->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
    ui->logo1->setPixmap(pix1->scaled(ui->logo1->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
    if (!myPlayerNorth->loadVideo("../TrafficGUI/CarsDrivingUnderBridge.mp4"))
    {
        QMessageBox msgBox;
        msgBox.setText("The selected video could not be opened!");
        msgBox.exec();
    }
    if (!myPlayerSouth->loadVideo("../TrafficGUI/CarsDrivingUnderBridge1.mp4"))
    {
        QMessageBox msgBox;
        msgBox.setText("The selected video could not be opened!");
        msgBox.exec();
    }
    if (!myPlayerEast->loadVideo("../TrafficGUI/CarsDrivingUnderBridge2.mp4"))
    {
        QMessageBox msgBox;
        msgBox.setText("The selected video could not be opened!");
        msgBox.exec();
    }
    if (!myPlayerWest->loadVideo("../TrafficGUI/CarsDrivingUnderBridge3.mp4"))
    {
        QMessageBox msgBox;
        msgBox.setText("The selected video could not be opened!");
        msgBox.exec();
    }

    myPlayerNorth->set_position(0.35,0.15,0.20,0.85,0,0.2999);
    myPlayerSouth->set_position(0.40,0.30,0.1,0.58,0.5701,0.80);
    myPlayerEast->set_position(0.80,0.85,0.16,0.45,0.5201,0.97);
    myPlayerWest->set_position(0.35,0.30,0.25,0.75,0,0.2899);
//    time_South->timer->start(1000);
//    time_East->timer->start(1000);
//    time_West->timer->start(1000);
    widget_North->Start();
    widget_South->Start();
    widget_East->Start();
    widget_West->Start();
    widget_North->ChangetoRed();
    widget_South->ChangetoRed();
    widget_East->ChangetoRed();
    widget_West->ChangetoRed();
    time_North->timer->start(1000);
    myPlayerNorth->Play();
    myPlayerSouth->Play();
    myPlayerEast->Play();
    myPlayerWest->Play();
}

void MainWindow::greenTimeCalculator_point()
{
    if(this->DTMS)
    {
        greenTimeCalculator();
    }
    else
    {
        greenTimeNormal();
    }
}

static void Init_params()
{
    para_road[0].count_of_cars=5;
    para_road[0].green_lit=1;
    para_road[0].green_time=0;
    para_road[0].wait_time=0;
    para_road[0].masked=1;
    para_road[0].prev_count_of_cars=0;
    for(int i=1;i<4;i++){
            para_road[i].count_of_cars=5;
            para_road[i].green_lit=0;
            para_road[i].green_time=0;
            para_road[i].wait_time=0;
            para_road[i].masked=0;
            para_road[i].prev_count_of_cars=0;
        }
}

void MainWindow::updatePlayerUI(QImage img)
{
    if (!img.isNull())
    {
        ui->videoOP->setAlignment(Qt::AlignCenter);
        ui->videoOP->setPixmap(QPixmap::fromImage(img).scaled(ui->videoOP->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

void MainWindow::updatePlayerUI1(QImage img)
{
    if (!img.isNull())
    {
        ui->videoOP1->setAlignment(Qt::AlignCenter);
        ui->videoOP1->setPixmap(QPixmap::fromImage(img).scaled(ui->videoOP->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

void MainWindow::updatePlayerUI2(QImage img)
{
    if (!img.isNull())
    {
        ui->videoOP2->setAlignment(Qt::AlignCenter);
        ui->videoOP2->setPixmap(QPixmap::fromImage(img).scaled(ui->videoOP->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

void MainWindow::updatePlayerUI3(QImage img)
{
    if (!img.isNull())
    {
        ui->videoOP3->setAlignment(Qt::AlignCenter);
        ui->videoOP3->setPixmap(QPixmap::fromImage(img).scaled(ui->videoOP->size(),Qt::KeepAspectRatio, Qt::FastTransformation));
    }
//    emit countAvailable();
}

void MainWindow::greenTimeCalculator()
{
    int i,j;
    int sum_of_elems;
    int already_done[4]= {0,0,0,0};
    para_road[0].count_of_cars=myPlayerNorth->carCount1;
    para_road[1].count_of_cars=myPlayerEast->carCount1;
    para_road[2].count_of_cars=myPlayerSouth->carCount1;
    para_road[3].count_of_cars=myPlayerWest->carCount1;
    for(i=0;i<4;i++)
    {
        if((para_road[i].green_lit))
            break;
    }
    switch(i)
    {
        case 0: {
        widget_North->ChangetoRed();
        if((para_road[0].count_of_cars-=para_road[0].prev_count_of_cars)<0)
            para_road[0].count_of_cars=0;
        myPlayerNorth->carCount1= para_road[0].count_of_cars;
    }break;
    case 2: {
    widget_South->ChangetoRed();
    if((para_road[2].count_of_cars-=para_road[2].prev_count_of_cars)<0)
        para_road[2].count_of_cars=0;
    myPlayerSouth->carCount1= para_road[2].count_of_cars;
}break;
    case 1: {
    widget_East->ChangetoRed();
    if((para_road[1].count_of_cars-=para_road[1].prev_count_of_cars)<0)
        para_road[1].count_of_cars=0;
    myPlayerEast->carCount1= para_road[1].count_of_cars;
}break;
    case 3: {
    widget_West->ChangetoRed();
    if((para_road[3].count_of_cars-=para_road[3].prev_count_of_cars)<0)
        para_road[3].count_of_cars=0;
    myPlayerWest->carCount1= para_road[3].count_of_cars;
}break;
    }

    int temp[4];
        int b,c,d,k;
        for(i=0;i<4;i++){
            temp[i]=para_road[i].count_of_cars;
        }

        std::vector<int> myvector (temp, temp+4);
        std::sort (myvector.begin(), myvector.end(),std::greater<int>());
        b=myvector[0];
        for(i=0;i<4;i++){
            if(temp[i]==b){
                c=i;
                break;
            }
        }

        if((para_road[0].masked==1)   \
           & (para_road[1].masked==1) \
           & (para_road[2].masked==1) \
           & (para_road[3].masked==1)){
            for(i=0;i<4;i++){
                para_road[i].masked=0;
            }
        }

        for(i=0;i<4;i++){
            if(c==i){
                if(!para_road[i].masked){
                    para_road[i].green_lit = 1;
                    para_road[i].masked    = 1;
                    sum_of_elems = std::accumulate(myvector.begin(), myvector.end(), 0);
                    d = ((float)b/(float)(1+sum_of_elems))*global_signal_time;
                    if(d<5){
                        d=5;
                    }
                    para_road[i].wait_time=0;
                    para_road[i].green_time=d;
                    already_done[i]=1;
                }
                else
                {
                    if(b==myvector[0]){
                        b=myvector[1];
                    }
                    else if(b==myvector[1]){
                        b=myvector[2];
                    }
                    else if(b==myvector[2]){
                        b=myvector[3];
                    }
                    else
                        break;

                    for(i=0;i<4;i++){
                        if(temp[i]==b){
                            c=i;
                            break;
                        }
                    }

                    if(para_road[c].masked){
                        for(j=0;j<4;j++){
                            if(temp[j]==b && !para_road[j].masked){
                                c=j;
                                break;
                            }
                        }

                        for(i=0;i<4;i++){
                            if(temp[i]==b){
                                k=i;
                                break;
                            }
                        }

                        if(c == k){
                            for(j=0;j<4;j++){
                                if(temp[j]!=b && !para_road[j].masked){
                                    c=j;
                                    break;
                                }
                            }
                            b=temp[c];
                        }
                    }

                    i=-1;
                    continue;
                }
            }
        }

        for(i=0;i<4;i++){
            if(!already_done[i]){
                para_road[i].green_lit = 0;
                para_road[i].wait_time = d;
                para_road[i].green_time= 0;
                already_done[i]=1;
            }
        }
        for(i=0;i<4;i++)
        {
            if((para_road[i].green_lit))
                break;
        }
        int minutes_green = d/60;
        int seconds_green = d%60;
        switch(i)
        {
            case 0: {
            widget_North->ChangetoGreen();
            para_road[0].prev_count_of_cars=d;
            time_North->timeValue->setHMS(0,minutes_green,seconds_green);
            time_North->timer->start(1000);
        }break;
        case 2: {
        widget_South->ChangetoGreen();
        para_road[2].prev_count_of_cars=d;
        time_South->timeValue->setHMS(0,minutes_green,seconds_green);
        time_South->timer->start(1000);
    }break;
        case 1: {
        widget_East->ChangetoGreen();
        para_road[1].prev_count_of_cars=d;
        time_East->timeValue->setHMS(0,minutes_green,seconds_green);
        time_East->timer->start(1000);
    }break;
        case 3: {
        widget_West->ChangetoGreen();
        para_road[3].prev_count_of_cars=d;
        time_West->timeValue->setHMS(0,minutes_green,seconds_green);
        time_West->timer->start(1000);
    }break;
        }
}

void MainWindow::greenTimeNormal()
{
    int i;
    int d;
    para_road[0].count_of_cars=myPlayerNorth->carCount1;
    para_road[1].count_of_cars=myPlayerEast->carCount1;
    para_road[2].count_of_cars=myPlayerSouth->carCount1;
    para_road[3].count_of_cars=myPlayerWest->carCount1;
    for(i=0;i<4;i++)
    {
        if((para_road[i].green_lit))
            break;
    }
    switch(i)
    {
        case 0: {
        widget_North->ChangetoRed();
        para_road[0].green_lit= 0;
        if((para_road[0].count_of_cars-=para_road[0].prev_count_of_cars)<0)
            para_road[0].count_of_cars=0;
        myPlayerNorth->carCount1= para_road[0].count_of_cars;
    }break;
    case 2: {
    widget_South->ChangetoRed();
    para_road[2].green_lit= 0;
    if((para_road[2].count_of_cars-=para_road[2].prev_count_of_cars)<0)
        para_road[2].count_of_cars=0;
    myPlayerSouth->carCount1= para_road[2].count_of_cars;
}break;
    case 1: {
    widget_East->ChangetoRed();
    para_road[1].green_lit= 0;
    if((para_road[1].count_of_cars-=para_road[1].prev_count_of_cars)<0)
        para_road[1].count_of_cars=0;
    myPlayerEast->carCount1= para_road[1].count_of_cars;
}break;
    case 3: {
    widget_West->ChangetoRed();
    para_road[3].green_lit= 0;
    if((para_road[3].count_of_cars-=para_road[3].prev_count_of_cars)<0)
        para_road[3].count_of_cars=0;
    myPlayerWest->carCount1= para_road[3].count_of_cars;
}break;
    }


        if((para_road[0].masked==1)   \
           & (para_road[1].masked==1) \
           & (para_road[2].masked==1) \
           & (para_road[3].masked==1)){
            for(i=0;i<4;i++){
                para_road[i].masked=0;
            }
        }

        for(i=0;i<4;i++){
                if(!para_road[i].masked){
                    para_road[i].green_lit = 1;
                    para_road[i].masked    = 1;
                    d = 30;
                    para_road[i].wait_time=0;
                    para_road[i].green_time=d;
                    break;
                }
        }

        for(i=0;i<4;i++)
        {
            if((para_road[i].green_lit))
                break;
        }
        int minutes_green = d/60;
        int seconds_green = d%60;
        switch(i)
        {
            case 0: {
            widget_North->ChangetoGreen();
            para_road[0].prev_count_of_cars=d;
            time_North->timeValue->setHMS(0,minutes_green,seconds_green);
            time_North->timer->start(1000);
        }break;
        case 2: {
        widget_South->ChangetoGreen();
        para_road[2].prev_count_of_cars=d;
        time_South->timeValue->setHMS(0,minutes_green,seconds_green);
        time_South->timer->start(1000);
    }break;
        case 1: {
        widget_East->ChangetoGreen();
        para_road[1].prev_count_of_cars=d;
        time_East->timeValue->setHMS(0,minutes_green,seconds_green);
        time_East->timer->start(1000);
    }break;
        case 3: {
        widget_West->ChangetoGreen();
        para_road[3].prev_count_of_cars=d;
        time_West->timeValue->setHMS(0,minutes_green,seconds_green);
        time_West->timer->start(1000);
    }break;
        }
}

MainWindow::~MainWindow()
{
    delete time_East;
    delete time_North;
    delete time_South;
    delete time_West;
    delete myPlayerNorth;
    delete myPlayerSouth;
    delete myPlayerEast;
    delete myPlayerWest;
    delete widget_North;
    delete widget_West;
    delete widget_East;
    delete widget_South;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    this->DTMS= true;
    ui->textEdit->setText("Current running algorithm is DTMS");
}

void MainWindow::on_pushButton_2_clicked()
{
    this->DTMS= false;
    ui->textEdit->setText("Current running algorithm is normal");
}
