#include <fstream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "HandGesture.hpp"

namespace HandGesture{
void HandGesture::landmarkToGesture()
{
    gameMode();
    #if PER_MODE
    performaceMode();
    #endif
}
void HandGesture::angleSimilarity(std::array<int, ShmConfig::handNum> &gesReturn)
{
    for(int hand=0; hand<multiHandNum; hand++){
        int maxGes = -1;
        float maxSim = 200;

        for(int ges=0; ges<gestureDef.size(); ges++){
            float gesSim = 0.f;
            for(int cmpAngleArrCnt=0; cmpAngleArrCnt<cmpAngleArrNum; cmpAngleArrCnt++){
                int joint = cmpAngleArr[cmpAngleArrCnt];
                gesSim += fabs(landmarks[hand][joint].angle - gestureDef[ges][joint].angle);
            }
            //std::cout << "hand: " << hand << " ges: " << ges << " gesSim: " << gesSim << std:: endl;
            if(gesSim < maxSim){
                maxSim = gesSim;
                maxGes = ges;
            }
        }
        //std::cout << "hand: " << hand << " maxGes: " << maxGes << " maxSim: " << maxSim << std:: endl;
        if(maxSim > angleSimilarityThreshold){
            maxGes = -1;
        }
        gesReturn[hand] = maxGes;
    }
}
void HandGesture::initCmpAngleArr()
{
    for(int i=0; i<cmpAngleArrNum; i++){
        cmpAngleArr[i] = (i%3) + (i/3)*4 + 1;
    }
}
template <class Lm2DArrayVec>
void HandGesture::initJointCosAngle(Lm2DArrayVec &lm, const int &idxNum)
{
    for(int idx=0; idx<idxNum; idx++){
        for(int i=0; i<cmpAngleArrNum; i++){
            int joint = cmpAngleArr[i];
            lm[idx][joint].cosAngle(lm[idx][joint-1], lm[idx][joint+1]);
        }
    }
}
template <class Lm2DArrayVec>
void HandGesture::initJointSinAngle(Lm2DArrayVec &lm, const int &idxNum)
{
    for(int idx=0; idx<idxNum; idx++){
        for(int i=0; i<cmpAngleArrNum; i++){
            int joint = cmpAngleArr[i];
            lm[idx][joint].sinAngle(lm[idx][joint-1], lm[idx][joint+1]);
        }
    }
}
void HandGesture::initImageSize()
{
    cv::VideoCapture capture(camID);
    capture.open(camID);
    if (!capture.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        exit(EXIT_FAILURE);
    }
    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    capture.set(cv::CAP_PROP_FOURCC, fourcc);
    capture.set(cv::CAP_PROP_FRAME_WIDTH, camWidth);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, camHeight);
    capture.set(cv::CAP_PROP_FPS, fps);
    imageSize = {camWidth, camHeight, 1};
	std::cout << "setting fps: " << capture.get(cv::CAP_PROP_FPS) << std::endl;
}
template <class Lm2DArrayVec>
void HandGesture::resize(Lm2DArrayVec &lm, const int &idxNum)
{
    for(int i=0; i<idxNum; i++){
        for(int j=0; j<ShmConfig::jointNum; j++){
            lm[i][j] = lm[i][j] * imageSize;
        }
    }
}
void HandGesture::initGestureDef()
{    
    // load defined gestures
    for(int ges=0; ges<gestureNum; ges++){
        const std::string path = gesturePath + "/" + std::to_string(ges)+".gesture";

        std::ifstream gesFile(path);
        if(!gesFile){
            std::cerr << "load_gesture failed\n";
            exit(EXIT_FAILURE);
        }

        std::string s;
        lmArray lmarr;
        for(int joint=0; joint<ShmConfig::jointNum; joint++){
            std::getline(gesFile, s);
            std::istringstream ss(s);
            int index;

            ss >> index >> lmarr[joint];
        }
        gestureDef.push_back(lmarr);
        std::getline(gesFile, s);
        // cause error because initGestureName() after this function
        gestureName.push_back(s);
        
        gesFile.close();
    }

    preprocess(gestureDef, gestureDef.size());
/*
    const std::string anglePath = gesturePath + "/all.angle";
    std::ofstream angleFile(anglePath);
    for(int i=0; i<gestureDef.size(); i++){
        angleFile << i << std::endl;
        for(const auto &lm : gestureDef[i]){
            angleFile << lm << std::endl;
        }
        angleFile << std::endl;
    }
    angleFile.close();
*/
}
template <class Lm2DArrayVec>
void HandGesture::preprocess(Lm2DArrayVec &lm, const int &idxNum)
{
    resize(lm, idxNum);

    initJointSinAngle(lm, idxNum);
}
void HandGesture::defineMode(cv::Mat &output_frame_mat, lm2DArray &lmCopy)
{
    int defineGestureNum {0};
    std::cout << "Input gesture ID: \n";
    std::cin >> defineGestureNum;
    std::cout << gesturePath << std::endl;
    const std::string path = gesturePath + "/" + std::to_string(defineGestureNum);
    std::ofstream gestureFile(path + ".gesture");

    if(!gestureFile){
        std::cerr << "file open failed: " << path << ".gesture" << std::endl;
    }

    for(int i=0; i<ShmConfig::jointNum; i++){
        gestureFile << i << " " << lmCopy[0][i].x << " " << lmCopy[0][i].y << " " << lmCopy[0][i].z << std::endl;
    }

    std::string defineGestureName;
    std::cout << "Input gesture name: \n";
    std::cin >> defineGestureName;
    gestureFile << defineGestureName << std::endl;

    gestureFile.close();

    // preprocess
    preprocess(lmCopy, 1);

    // store files
    std::ofstream gestureLogFile(path + ".gestureLog");

    if(!gestureLogFile){
        std::cerr << "file open failed: " << path << ".gestureLog" << std::endl;
    }

    for(int i=0; i<ShmConfig::jointNum; i++){
        gestureLogFile << i << lmCopy[0][i] << std::endl;
    }

    gestureLogFile.close();

    // save picture
    cv::imwrite(path + ".jpg", output_frame_mat);
}
void HandGesture::gameMode()
{
    std::array<int, ShmConfig::handNum> ges;

    preprocess(landmarks, multiHandNum);

    angleSimilarity(ges);

    // send data
    gesture->outputHandNum = multiHandNum;
    // assume that multiRectNum must larger than multiHandNum
    // save gesture to shared memory
    for(int hand=0; hand<multiHandNum; hand++){
        bbCenter[hand].gesture = ges[hand];
    }
    gesture->lm = bbCenter;

}
void HandGesture::performaceMode()
{
    if(perCnt == 0){
        clock_gettime(CLOCK_MONOTONIC_COARSE, &start);
        ++perCnt;
    }
    else if(perCnt == perCntMax){
		perCnt = 0;
        clock_gettime(CLOCK_MONOTONIC_COARSE, &end);
        struct timespec temp = diff(start, end);
        double gesture_time = (temp.tv_sec + (double) temp.tv_nsec / 1000000000.0);
        double gesture_fps = perCntMax / gesture_time;

        std::cout << "FPS: " << gesture_fps << std::endl; 
    }
    else{
        ++perCnt;
    }
}
}
