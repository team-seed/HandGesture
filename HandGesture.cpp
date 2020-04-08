#include "HandGesture.hpp"

//#include <boost/interprocess/managed_shared_memory.hpp>

namespace HandGesture{
HandGesture::HandGesture(ShmConfig::Gesture *gesturePtr)
:gesture(gesturePtr)
{
    initLandmark();
    initbbCenter();
    getHandGestureConfig();

    // init from l2g HandGesture
    initCmpAngleArr();
    initImageSize();
    initGestureName();
    initGestureDef();
}
HandGesture::~HandGesture()
{
}
/*
void HandGesture::HandGesture::initShm()
{
    boost::interprocess::shared_memory_object::remove(shmName);

    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, shmName, shmSize);

    // Construct an variable in shared memory
    segment.construct<ShmConfig::Gesture>(shmbbCenterGestureName)[ShmConfig::handNum]();
}
void HandGesture::HandGesture::delShm()
{
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_only, shmName);

    //When done, destroy gesture from the segment
    segment.destroy<ShmConfig::Gesture>(shmbbCenterGestureName);

    boost::interprocess::shared_memory_object::remove(shmName);
}
void HandGesture::HandGesture::openShm()
{
    cout << "* openShm" << endl;

    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, shmName, shmSize);

    // Construct an variable in shared memory
    segment.find<ShmConfig::Gesture>(
        shmbbCenterGestureName).first;
}
*/
void HandGesture::initLandmark()
{
    landmarks = new ShmConfig::Landmark*[ShmConfig::handNum];
    for(int i=0; i<ShmConfig::handNum; i++){
        landmarks[i] = new ShmConfig::Landmark[ShmConfig::jointNum];
    }
}
void HandGesture::getHandGestureConfig()
{
    if(perCnt == 0){
        clock_gettime(CLOCK_MONOTONIC_COARSE, &start);
    }
    else if(perCnt > perCntMax){
        clock_gettime(CLOCK_MONOTONIC_COARSE, &end);
        struct timespec temp = diff(start, end);
        double gesture_time = (temp.tv_sec + (double) temp.tv_nsec / 1000000000.0);
        double gesture_fps = perCntMax / gesture_time;

        std::cout << "FPS: " << gesture_fps << std::endl;
    }
    ++perCnt;
}
void HandGesture::initbbCenter()
{
    bbCenter = new ShmConfig::Landmark[ShmConfig::handNum];
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