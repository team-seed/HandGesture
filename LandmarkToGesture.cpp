#include <fstream>
#include <string>

#include "HandGesture.hpp"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

namespace HandGesture{
void HandGesture::landmarkToGesture()
{
    #if GAME_MODE
    gameMode();
    #elif PER_MODE
    gameMode();
    performaceMode();
    #endif
}
void HandGesture::angleSimilarity(int *gesReturn)
{
    for(int hand=0; hand<multiHandNum; hand++){
        int maxGes = -1;
        float maxSim = 200;

        for(int ges=0; ges<gestureNum; ges++){
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
    cmpAngleArr = new int [cmpAngleArrNum];
    for(int i=0; i<cmpAngleArrNum; i++){
        cmpAngleArr[i] = (i%3) + (i/3)*4 + 1;
    }
}
void HandGesture::initJointAngle(ShmConfig::Landmark **lm, const int &idxNum)
{
    for(int idx=0; idx<idxNum; idx++){
        for(int i=0; i<cmpAngleArrNum; i++){
            int joint = cmpAngleArr[i];
            lm[idx][joint].cosAngle(lm[idx][joint-1], lm[idx][joint+1]);
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
    const float w = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    const float h = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    imageSize = {w, h, 1};
}
void HandGesture::resize(ShmConfig::Landmark **lm, const int &idxNum)
{
    for(int i=0; i<idxNum; i++){
        for(int j=0; j<ShmConfig::jointNum; j++){
            lm[i][j] = lm[i][j] * imageSize;
        }
    }
}
void HandGesture::initGestureDef()
{
    // allocate memory
    gestureDef = new ShmConfig::Landmark*[gestureNum];
    for(int ges=0; ges<gestureNum; ges++){
        gestureDef[ges] = new ShmConfig::Landmark[ShmConfig::jointNum];
    }
    
    // load defined gestures
    for(int ges=0; ges<gestureNum; ges++){
        const std::string path = std::string(gesturePath) + "/" + std::to_string(ges)+".gesture";

        std::ifstream gesFile(path);
        if(!gesFile){
            std::cerr << "load_gesture";
            exit(EXIT_FAILURE);
        }

        std::string s;
        for(int joint=0; joint<ShmConfig::jointNum; joint++){
            std::getline(gesFile, s);
            std::istringstream ss(s);
            int index;

            ss >> index >> gestureDef[ges][joint];
        }
        std::getline(gesFile, s);
        // cause error because initGestureName() after this function
        gestureName[ges] = s;
        
        gesFile.close();
    }

    preprocess(gestureDef, gestureNum);
}
void HandGesture::preprocess(ShmConfig::Landmark **lm, const int &idxNum)
{
    resize(lm, idxNum);

    initJointAngle(lm, idxNum);
}
void HandGesture::initGestureName()
{
    gestureName = new std::string[gestureNum];
}
void HandGesture::defineMode(cv::Mat &output_frame_mat)
{
    int defineGestureNum {0};
    std::cout << "Input gesture ID: \n";
    std::cin >> defineGestureNum;
    std::string path(gesturePath + "/" + std::to_string(defineGestureNum) + ".gesture");
    std::ofstream gestureFile(path);

    if(!gestureFile){
        std::cerr << "file open failed: " << path << std::endl;
    }

    for(int i=0; i<ShmConfig::jointNum; i++){
        gestureFile << i << " " << landmarks[0][i].x << " " << landmarks[0][i].y << " " << landmarks[0][i].z << std::endl;
    }

    std::string defineGestureName;
    std::cout << "Input gesture name: \n";
    std::cin >> defineGestureName;
    gestureFile << defineGestureName << std::endl;

    gestureFile.close();

    // preprocess
    preprocess(landmarks, 1);

    // store files
    std::string pathLog(gesturePath + "/" + std::to_string(defineGestureNum) + ".gestureLog");
    std::ofstream gestureLogFile(pathLog);

    if(!gestureLogFile){
        std::cerr << "file open failed: " << pathLog << std::endl;
    }

    for(int i=0; i<ShmConfig::jointNum; i++){
        gestureLogFile << i << " " << landmarks[0][i].x << " " << landmarks[0][i].y << std::endl;
    }

    gestureLogFile.close();

    // save picture
    cv::imwrite(gesturePath + "/" + std::to_string(defineGestureNum) + ".jpg", output_frame_mat);
}
void HandGesture::gameMode()
{
    int *ges = new int[multiHandNum];

    preprocess(landmarks, multiHandNum);

    angleSimilarity(ges);

    // assume that multiRectNum must larger than multiHandNum
    // save gesture to shared memory
    for(int hand=0; hand<multiHandNum; hand++){
        gesture[hand] = {bbCenter[hand], ges[hand]};
    }
    // if multiHandNum smaller than config.ShmConfig::handNum, set others to ges[0]
    for(int hand=multiHandNum; hand<ShmConfig::handNum; hand++){
        gesture[hand].gesture = ges[0];
    }
    // if multiRectNum smaller than config.ShmConfig::handNum, set others to gesture[0].lm
    for(int rect=multiHandNum; rect<ShmConfig::handNum; rect++){
        gesture[rect].lm = bbCenter[0];
    }
}
void HandGesture::performaceMode()
{
    if(perCnt == 0){
        clock_gettime(CLOCK_MONOTONIC_COARSE, &start);
    }
    else if(perCnt == perCntMax){
        clock_gettime(CLOCK_MONOTONIC_COARSE, &end);
        struct timespec temp = diff(start, end);
        double gesture_time = (temp.tv_sec + (double) temp.tv_nsec / 1000000000.0);
        double gesture_fps = perCntMax / gesture_time;

        std::cout << "FPS: " << gesture_fps << std::endl; 
    }
    ++perCnt;
}
}