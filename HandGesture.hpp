#ifndef HANDGESTURE_HPP
#define HANDGESTURE_HPP

#include <iostream>
#include <string>
#include <cmath>
#include <ctime>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "ShmConfig.hpp"

namespace HandGesture{
    // POSIX time measurement
    struct timespec diff(struct timespec start, struct timespec end);
    class HandGesture{
    public:
            void landmarkToGesture();
            void angleSimilarity(int *gesReturn);
            void resize(ShmConfig::Landmark **lm, const int &idxNum);
            void preprocess(ShmConfig::Landmark **lm, const int &idxNum);

            void initCmpAngleArr();
            void initJointAngle(ShmConfig::Landmark **lm, const int &idxNum);
            void initImageSize();
            void initGestureDef();
            void initGestureName();

            int *cmpAngleArr;
            int cmpAngleArrNum = 15;
            ShmConfig::Landmark **gestureDef;
            ShmConfig::Landmark imageSize;
            std::string *gestureName;

            int fps = 60;
            int gestureNum = 10;
            float angleSimilarityThreshold = 2.0f;
            int camID = 0;
            std::string gesturePath = "../mediapipe/mediapipe/HandGesture/store_gesture";
            int perCnt = 0;
            int perCntMax = 1e5;

        // provide these four functions API for NeoHand
        //void handGestureConfig();  // NeoHand related config, implement by file IO
        void recvGesture();
        void recvPosition();

        // provide these API to get and set variables which must consider IPC issues
        void getGesture();   // aka recvGesture
        void setGesture();
        void getbbCenter();  // aka recvPosition
        void setbbCenter();
        void getHandGestureConfig();  // get all config from NeoHand and HandGesture
        void setHandGestureConfig();  // set all config from NeoHand and HandGesture

        // init shm
        //void initShm();
        //void delShm();
        //void openShm();

        // internal class using
        HandGesture(ShmConfig::Gesture *gesturePtr);
        ~HandGesture();
        void initLandmark();
        void initbbCenter();
        void defineMode(cv::Mat &output_frame_mat);
        void gameMode();
        void performaceMode();

    //private:
        ShmConfig::Landmark **landmarks;
        ShmConfig::Landmark *bbCenter;
        
        // hand number from mediapipe multi hand
        int multiHandNum;
        int multiRectNum;
        
        // these three variables must consider IPC issues
        ShmConfig::Gesture *gesture;
        struct timespec start, end;
    };
}

#endif