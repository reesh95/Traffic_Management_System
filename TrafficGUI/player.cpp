#include "player.h"

// global variables ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

// function prototypes ////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs);
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex);
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs);
double distanceBetweenPoints(cv::Point point1, cv::Point point2);
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName);
bool checkIfBlobsCrossedTheLineLane1(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &intVerticalLinePositionStart, int &intVerticalLinePositionEnd, int &carCount);
bool checkIfBlobsCrossedTheLineLane2(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &intVerticalLinePositionStart,int &intVerticalLinePositionEnd, int &carCount);
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy);
void drawCarCountOnImage(int &carCount, int &carCount1, cv::Mat &imgFrame2Copy);

void Player::set_position(float hor_line_pos_div,
 float hor_line_pos_div1,
 float ver_line_pos_start,
 float ver_line_pos_end,
 float ver_line_pos_start1,
 float ver_line_pos_end1)
{
    this->hor_line_pos_div= hor_line_pos_div;
    this->hor_line_pos_div1= hor_line_pos_div1;
    this->ver_line_pos_start= ver_line_pos_start;
    this->ver_line_pos_end= ver_line_pos_end;
    this->ver_line_pos_start1= ver_line_pos_start1;
    this->ver_line_pos_end1= ver_line_pos_end1;

}

Player::Player(QObject *parent)
 : QThread(parent)
{
    carCount1 = 0;
    carCount2 = 0;
    frameCount = 2;
    stop = true;
}

bool Player::loadVideo(String filename) {
    capVideo.open(filename);
    if (capVideo.isOpened())
    {
        frameRate = (int) capVideo.get(CV_CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

void Player::Play()
{
    if (!isRunning()) {
        if (isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}

void Player::run()
{
    int delay = (1000/frameRate);
    std::vector<Blob> currentFrameBlobs;
    bool blnFirstFrame = true;
    cv::Point crossingLine[2];
    cv::Point crossingLine1[2];
    if (!(capVideo.read(imgFrame1) && capVideo.read(imgFrame2)))
    {
        stop = true;
    }
    intHorizontalLinePosition = (int)std::round((double)imgFrame1.rows * hor_line_pos_div);
    intVerticalLinePositionStart= (int) std::round((double)imgFrame1.cols * ver_line_pos_start);
    intVerticalLinePositionEnd= (int) std::round((double)imgFrame1.cols * ver_line_pos_end);
    intHorizontalLinePosition1 = (int)std::round((double)imgFrame1.rows * hor_line_pos_div1);
    intVerticalLinePositionStart1= (int) std::round((double)imgFrame1.cols * ver_line_pos_start1);
    intVerticalLinePositionEnd1= (int) std::round((double)imgFrame1.cols * ver_line_pos_end1);
    crossingLine[0].x = intVerticalLinePositionStart;
    crossingLine[0].y = intHorizontalLinePosition;

    crossingLine[1].x = intVerticalLinePositionEnd;
    crossingLine[1].y = intHorizontalLinePosition;
    crossingLine1[0].x = intVerticalLinePositionStart1;
    crossingLine1[0].y = intHorizontalLinePosition1;

    crossingLine1[1].x = intVerticalLinePositionEnd1;
    crossingLine1[1].y = intHorizontalLinePosition1;
    while(!stop){

        cv::Mat imgFrame1Copy = imgFrame1.clone();
        imgFrame2Copy = imgFrame2.clone();

        cv::Mat imgDifference;
        cv::Mat imgThresh;

        cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
        cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

               cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
               cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);

               cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

               cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);

               //cv::imshow("imgThresh", imgThresh);

               cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
               cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
               cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
               cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

               for (unsigned int i = 0; i < 2; i++) {
                   cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                   cv::dilate(imgThresh, imgThresh, structuringElement5x5);
                   cv::erode(imgThresh, imgThresh, structuringElement5x5);
               }

               cv::Mat imgThreshCopy = imgThresh.clone();

               std::vector<std::vector<cv::Point> > contours;

               cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

               //drawAndShowContours(imgThresh.size(), contours, "imgContours");

               std::vector<std::vector<cv::Point> > convexHulls(contours.size());

               for (unsigned int i = 0; i < contours.size(); i++) {
                   cv::convexHull(contours[i], convexHulls[i]);
               }

               //drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

               for (auto &convexHull : convexHulls) {
                   Blob possibleBlob(convexHull);

                   if (possibleBlob.currentBoundingRect.area() > 400 &&
                       possibleBlob.dblCurrentAspectRatio > 0.2 &&
                       possibleBlob.dblCurrentAspectRatio < 4.0 &&
                       possibleBlob.currentBoundingRect.width > 30 &&
                       possibleBlob.currentBoundingRect.height > 30 &&
                       possibleBlob.dblCurrentDiagonalSize > 60.0 &&
                       (cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50) {
                       currentFrameBlobs.push_back(possibleBlob);
                   }
               }

               //drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

               if (blnFirstFrame == true) {
                   for (auto &currentFrameBlob : currentFrameBlobs) {
                       blobs.push_back(currentFrameBlob);
                   }
               } else {
                   matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
               }

               //drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

               imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

               //drawBlobInfoOnImage(blobs, imgFrame2Copy);

               bool blnAtLeastOneBlobCrossedTheLineLane1 = checkIfBlobsCrossedTheLineLane1(blobs, intHorizontalLinePosition, intVerticalLinePositionStart,intVerticalLinePositionEnd, carCount1);

               if (blnAtLeastOneBlobCrossedTheLineLane1 == true) {
                   cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
               }
               else {
                   cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
               }

               bool blnAtLeastOneBlobCrossedTheLineLane2 = checkIfBlobsCrossedTheLineLane2(blobs, intHorizontalLinePosition1, intVerticalLinePositionStart1, intVerticalLinePositionEnd1, carCount2);

               if (blnAtLeastOneBlobCrossedTheLineLane2 == true) {
                   cv::line(imgFrame2Copy, crossingLine1[0], crossingLine1[1], SCALAR_GREEN, 2);
               }
               else {
                   cv::line(imgFrame2Copy, crossingLine1[0], crossingLine1[1], SCALAR_YELLOW, 2);
               }

               drawCarCountOnImage(carCount1, carCount2, imgFrame2Copy);

               //cv::imshow("imgFrame2Copy", imgFrame2Copy);

               if (imgFrame2Copy.channels()== 3){
                   cv::cvtColor(imgFrame2Copy, RGBframe, CV_BGR2RGB);
                   img = QImage((const unsigned char*)(RGBframe.data),
                                     RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
               }
               else
               {
                   img = QImage((const unsigned char*)(imgFrame2Copy.data),
                                        imgFrame2Copy.cols,imgFrame2Copy.rows,QImage::Format_Indexed8);
               }
               emit processedImage(img);

               //cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

                       // now we prepare for the next iteration

               currentFrameBlobs.clear();

               imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is

               if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT)) {
                   if(!capVideo.read(imgFrame2))
                   {
                       stop= true;
                   }
               }
           blnFirstFrame = false;
           frameCount++;
        this->msleep(delay);
    }
}

Player::~Player()
{
    mutex.lock();
    stop = true;
    capVideo.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}
void Player::Stop()
{
    stop = true;
}
void Player::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}
bool Player::isStopped() const{
    return this->stop;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs) {

    for (auto &existingBlob : existingBlobs) {

        existingBlob.blnCurrentMatchFoundOrNewBlob = false;

        existingBlob.predictNextPosition();
    }

    for (auto &currentFrameBlob : currentFrameBlobs) {

        int intIndexOfLeastDistance = 0;
        double dblLeastDistance = 100000.0;

        for (unsigned int i = 0; i < existingBlobs.size(); i++) {

            if (existingBlobs[i].blnStillBeingTracked == true) {

                double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

                if (dblDistance < dblLeastDistance) {
                    dblLeastDistance = dblDistance;
                    intIndexOfLeastDistance = i;
                }
            }
        }

        if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 0.5) {
            addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
        }
        else {
            addNewBlob(currentFrameBlob, existingBlobs);
        }

    }

    for (auto &existingBlob : existingBlobs) {

        if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
            existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
        }

        if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5) {
            existingBlob.blnStillBeingTracked = false;
        }

    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex) {

    existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
    existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;

    existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());

    existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
    existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;

    existingBlobs[intIndex].blnStillBeingTracked = true;
    existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs) {

    currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;

    existingBlobs.push_back(currentFrameBlob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double distanceBetweenPoints(cv::Point point1, cv::Point point2) {

    int intX = abs(point1.x - point2.x);
    int intY = abs(point1.y - point2.y);

    return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

    //cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName) {

    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

    std::vector<std::vector<cv::Point> > contours;

    for (auto &blob : blobs) {
        if (blob.blnStillBeingTracked == true) {
            contours.push_back(blob.currentContour);
        }
    }

    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

    //cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedTheLineLane1(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &intVerticalLinePositionStart,
                                     int &intVerticalLinePositionEnd,int &carCount) {
    bool blnAtLeastOneBlobCrossedTheLine = false;

    for (auto blob : blobs) {

        if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
            int prevFrameIndex = (int)blob.centerPositions.size() - 2;
            int currFrameIndex = (int)blob.centerPositions.size() - 1;

            if ((blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition)
                   && (blob.centerPositions[currFrameIndex].x <= intVerticalLinePositionEnd && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionStart)) {
                carCount++;
                blnAtLeastOneBlobCrossedTheLine = true;
            }
        }

    }

    return blnAtLeastOneBlobCrossedTheLine;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedTheLineLane2(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &intVerticalLinePositionStart,
                                     int &intVerticalLinePositionEnd, int &carCount) {
    bool blnAtLeastOneBlobCrossedTheLine = false;

    for (auto blob : blobs) {

        if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
            int prevFrameIndex = (int)blob.centerPositions.size() - 2;
            int currFrameIndex = (int)blob.centerPositions.size() - 1;

            if (((blob.centerPositions[prevFrameIndex].y < intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y >= intHorizontalLinePosition)
                        && (blob.centerPositions[currFrameIndex].x <= intVerticalLinePositionEnd && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionStart ))) {
                carCount++;
                blnAtLeastOneBlobCrossedTheLine = true;
            }
        }

    }

    return blnAtLeastOneBlobCrossedTheLine;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy) {

    for (unsigned int i = 0; i < blobs.size(); i++) {

        if (blobs[i].blnStillBeingTracked == true) {
            cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);

            int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
            double dblFontScale = blobs[i].dblCurrentDiagonalSize / 60.0;
            int intFontThickness = (int)std::round(dblFontScale * 1.0);

            cv::putText(imgFrame2Copy, std::to_string(i), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawCarCountOnImage(int &carCount, int &carCount1, cv::Mat &imgFrame2Copy) {

    int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
    double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
    int intFontThickness = (int)std::round(dblFontScale * 1.5);

    cv::Size textSize = cv::getTextSize(std::to_string(carCount), intFontFace, dblFontScale, intFontThickness, 0);

    cv::Point ptTextBottomLeftPosition;
    cv::Point ptTextTopLeftPosition;

    ptTextBottomLeftPosition.x = imgFrame2Copy.cols - 1 - (int)((double)textSize.width * 1.25);
    ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);
    ptTextTopLeftPosition.x = (int)((double)textSize.width * 1.25);
    ptTextTopLeftPosition.y = (int)((double)textSize.height * 1.25);

    cv::putText(imgFrame2Copy, std::to_string(carCount), ptTextBottomLeftPosition, intFontFace, dblFontScale, SCALAR_RED, intFontThickness);
    cv::putText(imgFrame2Copy, std::to_string(carCount1), ptTextTopLeftPosition, intFontFace, dblFontScale, SCALAR_YELLOW, intFontThickness);
}
