#ifndef HANDGESTURE_HPP
#define HANDGESTURE_HPP

#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <array>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "ShmConfig.hpp"

namespace HandGesture{
    // POSIX time measurement
    struct timespec diff(struct timespec start, struct timespec end);
    constexpr int cmpAngleArrNum {15};
    typedef std::array<ShmConfig::Landmark, ShmConfig::jointNum> lmArray;
    typedef std::array<lmArray, ShmConfig::handNum> lm2DArray;
    typedef std::array<ShmConfig::Normalized2DPoint, ShmConfig::handNum> pointArray;
    class HandGesture{
    public:
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

        // internal class using
        HandGesture(ShmConfig::Gesture *gesturePtr);
        ~HandGesture();
        void defineMode(cv::Mat &output_frame_mat, lm2DArray &lmCopy);
        void gameMode();
        void performaceMode();

    //private:
        lm2DArray landmarks;
        pointArray bbCenter;
        
        // hand number from mediapipe multi hand
        int multiHandNum;
        
        // these three variables must consider IPC issues
        ShmConfig::Gesture *gesture;
        struct timespec start, end;

            void landmarkToGesture();
            void angleSimilarity(std::array<int, ShmConfig::handNum> &gesReturn);
            template <class Lm2DArrayVec>
            void resize(Lm2DArrayVec &lm, const int &idxNum);
            template <class Lm2DArrayVec>
            void preprocess(Lm2DArrayVec &lm, const int &idxNum);

            void initCmpAngleArr();
            template <class Lm2DArrayVec>
            void initJointAngle(Lm2DArrayVec &lm, const int &idxNum);
            void initImageSize();
            void initGestureDef();

            std::array<int, cmpAngleArrNum> cmpAngleArr;
            std::vector<lmArray> gestureDef;
            ShmConfig::Landmark imageSize;
            // after config gestureNum
            std::vector<std::string> gestureName;
            std::vector<int> gestureID;

            int fps;
            int gestureNum;
            float angleSimilarityThreshold;
            int camID;
            std::string gesturePath;
            int perCnt;
            int perCntMax;
    };
}

#endif