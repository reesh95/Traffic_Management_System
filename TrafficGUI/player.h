#ifndef PLAYER_H
#define PLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <string.h>
#include <Blob.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
class Player : public QThread
{    Q_OBJECT
 private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    int frameRate;
    cv::VideoCapture capVideo;
    cv::Mat imgFrame1;
    cv::Mat imgFrame2;
    int intHorizontalLinePosition;
    int intHorizontalLinePosition1;
    int intVerticalLinePositionStart;
    int intVerticalLinePositionEnd;
    int intVerticalLinePositionStart1;
    int intVerticalLinePositionEnd1;
    cv::Mat imgFrame2Copy;
    cv::Mat RGBframe;
    std::vector<Blob> blobs;
    QImage img;
    int frameCount;
 signals:
 //Signal to output frame to be displayed
      void processedImage(const QImage &image);
 protected:
     void run();
     void msleep(int ms);
 public:
    //Constructor
    Player(QObject *parent = 0);
    //Destructor
    ~Player();
    //Load a video from memory
    bool loadVideo(String filename);
    //Play the video
    void Play();
    //Stop the video
    void Stop();
    //check if the player has been stopped
    bool isStopped() const;
    //set pos
    void set_position(float hor_line_pos_div=0.35,
     float hor_line_pos_div1=0.35,
     float ver_line_pos_start=0.35,
     float ver_line_pos_end=0.9999,
     float ver_line_pos_start1=0,
     float ver_line_pos_end1=0.3499);
    //Carcount
    int carCount1;
    int carCount2;
    //set horizontal line position
    float hor_line_pos_div;
    float hor_line_pos_div1;
    float ver_line_pos_start;
    float ver_line_pos_end;
    float ver_line_pos_start1;
    float ver_line_pos_end1;
};
#endif // PLAYER_H
