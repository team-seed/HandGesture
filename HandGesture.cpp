#include "HandGesture.hpp"

namespace HandGesture{
ostream& operator<<(ostream& o, const Landmark &l)
{
    return o << " (x, y, z) = " << l.x << ", " << l.y << ", " << l.z << " ";
}
ostream& operator<<(std::ostream& o, const Gesture &g)
{
    return o << " gesture " << g.lm << " is " << g.gesture << " ";
}

HandGesture::HandGesture::HandGesture()
{
    cout << "* HandGesture()" << endl;
    initLandmark();
}

void HandGesture::HandGesture::recvGesture()
{
    cout << "* recvGesture()" << endl;

    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        config.shmName.c_str(), config.shmSize);

    // Construct an variable in shared memory
    Gesture *gesture = segment.find<Gesture>(
        config.shmbbCenterGestureName.c_str()).first;

    for(int i=0; i<config.handNum; i++){
        cout << "receive gesture " << gesture[i] << endl;
    }
}
void HandGesture::HandGesture::initShm()
{
    boost::interprocess::shared_memory_object::remove(config.shmName.c_str());

    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        config.shmName.c_str(), config.shmSize);

    // Construct an variable in shared memory
    segment.construct<Gesture>(config.shmbbCenterGestureName.c_str())[config.handNum]();
}
void HandGesture::HandGesture::delShm()
{
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_only, config.shmName.c_str());

    //When done, destroy gesture from the segment
    segment.destroy<Gesture>(config.shmbbCenterGestureName.c_str());

    boost::interprocess::shared_memory_object::remove(config.shmName.c_str());
}
void HandGesture::HandGesture::openShm()
{
    cout << "* openShm" << endl;

    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        config.shmName.c_str(), config.shmSize);

    // Construct an variable in shared memory
    segment.find<Gesture>(
        config.shmbbCenterGestureName.c_str()).first;
}
void HandGesture::HandGesture::initLandmark()
{
    landmarks = new Landmark*[config.handNum];
    for(int i=0; i<config.handNum; i++){
        landmarks[i] = new Landmark[config.jointNum];
    }
}
void HandGesture::HandGesture::getHandGestureConfig()
{
    
}
}