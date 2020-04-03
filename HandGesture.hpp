#ifndef HANDGESTURE_HPP
#define HANDGESTURE_HPP

#include <iostream>
#include <string>
#include <cmath>

#include "ShmConfig.hpp"

namespace HandGesture{
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

    //private:
        ShmConfig::Landmark **landmarks;
        ShmConfig::Landmark *bbCenter;
        
        // hand number from mediapipe multi hand
        int multiHandNum;
        int multiRectNum;
        
        // these three variables must consider IPC issues
        ShmConfig::Gesture *gesture;
    };
}

#endif