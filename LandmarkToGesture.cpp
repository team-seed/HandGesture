#include <fstream>

#include "HandGesture.hpp"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

namespace HandGesture{
void HandGesture::HandGesture::landmarkToGesture()
{
    cout << "* landmarkToGesture()" << endl;

    int *ges = new int[multiHandNum];

    preprocess(landmarks, multiHandNum);

    angleSimilarity(ges);
/*
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        config.shmName.c_str(), config.shmSize);

    // Construct an variable in shared memory
    Gesture *gesture = segment.find<Gesture>(
        config.shmbbCenterGestureName.c_str()).first;

    if(gesture == 0){
        std::cout << "can't find shared memory\n";
    }
*/
    // assume that multiRectNum must larger than multiHandNum
    // save gesture to shared memory
    for(int hand=0; hand<multiHandNum; hand++){
        gesture[hand] = {bbCenter[hand], ges[hand]};
    }
    // if multiHandNum smaller than config.handNum, set others to ges[0]
    for(int hand=multiHandNum; hand<handNum; hand++){
        gesture[hand].gesture = ges[0];
    }
    // if multiRectNum smaller than config.handNum, set others to gesture[0].lm
    for(int rect=multiHandNum; rect<handNum; rect++){
        gesture[rect].lm = bbCenter[0];
    }
}
void HandGesture::HandGesture::angleSimilarity(int *gesReturn)
{
    cout << "* angleSimilarity()\n";
    for(int hand=0; hand<multiHandNum; hand++){
        int maxGes = -1;
        float maxSim = 200;

        for(int ges=0; ges<gestureNum; ges++){
            float gesSim = 0.f;
            for(int cmpAngleArrCnt=0; cmpAngleArrCnt<cmpAngleArrNum; cmpAngleArrCnt++){
                int joint = cmpAngleArr[cmpAngleArrCnt];
                gesSim += fabs(landmarks[hand][joint].angle - gestureDef[ges][joint].angle);
            }
            if(gesSim < maxSim){
                maxSim = gesSim;
                maxGes = ges;
            }
        }

        if(maxSim > angleSimilarityThreshold){
            maxGes = -1;
        }
        gesReturn[hand] = maxGes;
    }
}
void HandGesture::HandGesture::initCmpAngleArr()
{
    cmpAngleArr = new int [cmpAngleArrNum];
    for(int i=0; i<cmpAngleArrNum; i++){
        cmpAngleArr[i] = (i%3) + (i/3)*4 + 1;
    }
}
void HandGesture::HandGesture::initJointAngle(Landmark **lm, const int &idxNum)
{
    for(int idx=0; idx<idxNum; idx++){
        for(int i=0; i<cmpAngleArrNum; i++){
            int joint = cmpAngleArr[i];
            lm[idx][joint].cosAngle(lm[idx][joint-1], lm[idx][joint+1]);
        }
    }
}
void HandGesture::HandGesture::initImageSize()
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
void HandGesture::HandGesture::resize(Landmark **lm, const int &idxNum)
{
    for(int i=0; i<idxNum; i++){
        for(int j=0; j<jointNum; j++){
            lm[i][j] = lm[i][j] * imageSize;
        }
    }
}
void HandGesture::HandGesture::initGestureDef()
{
    cout << "* initGestureDef()\n";
    // allocate memory
    gestureDef = new Landmark*[gestureNum];
    for(int ges=0; ges<gestureNum; ges++){
        gestureDef[ges] = new Landmark[jointNum];
    }
    
    // load defined gestures
    for(int ges=0; ges<gestureNum; ges++){
        const std::string path = std::string(gesturePath) + "/" + std::to_string(ges)+".gesture";

        std::ifstream gesFile(path);
        if(!gesFile){
            std::perror("load_gesture");
            exit(EXIT_FAILURE);
        }

        std::string s;
        for(int joint=0; joint<jointNum; joint++){
            std::getline(gesFile, s);
            std::istringstream ss(s);
            int index; float angle;

            ss >> index >> gestureDef[ges][joint] >> angle;
        }
        std::getline(gesFile, s);
        // cause error
        //gestureName[ges] = s;
        
        gesFile.close();
    }
    preprocess(gestureDef, gestureNum);
}
void HandGesture::HandGesture::preprocess(Landmark **lm, const int &idxNum)
{
    cout << "* preprocess()\n";
    resize(lm, idxNum);

    initJointAngle(lm, idxNum);
}
void HandGesture::HandGesture::initGestureName()
{
    gestureName = new std::string[gestureNum];
}
}