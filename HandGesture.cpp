#include "HandGesture.hpp"

//#include <boost/interprocess/managed_shared_memory.hpp>

namespace HandGesture{
HandGesture::HandGesture::HandGesture(ShmConfig::Gesture *gesturePtr)
:gesture(gesturePtr)
{
    initLandmark();
    initbbCenter();
    getHandGestureConfig();

    // init from l2g HandGesture
    initCmpAngleArr();
    initGestureDef();
    initGestureName();
}
HandGesture::HandGesture::~HandGesture()
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
void HandGesture::HandGesture::initLandmark()
{
    landmarks = new ShmConfig::Landmark*[ShmConfig::handNum];
    for(int i=0; i<ShmConfig::handNum; i++){
        landmarks[i] = new ShmConfig::Landmark[ShmConfig::jointNum];
    }
}
void HandGesture::HandGesture::getHandGestureConfig()
{
    
}
void HandGesture::HandGesture::initbbCenter()
{
    bbCenter = new ShmConfig::Landmark[ShmConfig::handNum];
}
}