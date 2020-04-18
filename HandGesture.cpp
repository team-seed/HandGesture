#include "HandGesture.hpp"

namespace HandGesture{
HandGesture::HandGesture(ShmConfig::Gesture *gesturePtr)
:gesture{gesturePtr}, gestureNum{20},
angleSimilarityThreshold{2.0f},
camID{0}, perCnt{0}, perCntMax{3600},
gesturePath("../mediapipe/mediapipe/HandGesture/store_gesture")
{
    getHandGestureConfig();

    // init from l2g HandGesture
    initCmpAngleArr();
    initImageSize();
    initGestureDef();
}
HandGesture::~HandGesture()
{
}
void HandGesture::getHandGestureConfig()
{

}
struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}
}