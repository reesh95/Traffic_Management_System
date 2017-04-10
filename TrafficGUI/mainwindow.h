#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <player.h>
#include <traffic_light.h>
#include <QThread>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <LCDNumber.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void greenTimeCalculator();
    void greenTimeNormal();

private slots:
    //Display video frame in player UI
    void updatePlayerUI(QImage img);
    void updatePlayerUI1(QImage img);
    void updatePlayerUI2(QImage img);
    void updatePlayerUI3(QImage img);
    void greenTimeCalculator_point();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    bool DTMS;
    Player* myPlayerNorth;
    Player* myPlayerSouth;
    Player* myPlayerEast;
    Player* myPlayerWest;
    TrafficLight* widget_North;
    TrafficLight* widget_South;
    TrafficLight* widget_East;
    TrafficLight* widget_West;
    LCDNumber* time_North;
    LCDNumber* time_South;
    LCDNumber* time_East;
    LCDNumber* time_West;
};

#endif // MAINWINDOW_H
