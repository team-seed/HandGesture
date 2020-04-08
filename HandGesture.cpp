#include "HandGesture.hpp"

//#include <boost/interprocess/managed_shared_memory.hpp>

namespace HandGesture{
HandGesture::HandGesture(ShmConfig::Gesture *gesturePtr)
:gesture(gesturePtr)
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