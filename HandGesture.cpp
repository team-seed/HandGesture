#include "HandGesture.hpp"

namespace HandGesture{
ostream& operator<<(ostream& o, const Landmark &l)
{
    return o << " (x, y, z) = " << l.x << ", " << l.y << ", " << l.z << " ";
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
    gesture = segment.find<int>(
        config.shmGestureName.c_str()).first;

    // Construct an variable in shm
    bbCenter = segment.find<Landmark>(
        config.shmbbCenter.c_str()).first;

    cout << "receive gesture " << *gesture << endl;
}
void HandGesture::HandGesture::initShm()
{
    boost::interprocess::shared_memory_object::remove(config.shmName.c_str());

    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        config.shmName.c_str(), config.shmSize);

    // Construct an variable in shared memory
    gesture = segment.construct<int>(
        config.shmGestureName.c_str())();

    // Construct an variable in shm
    bbCenter = segment.construct<Landmark>(
        config.shmbbCenter.c_str())();
}
void HandGesture::HandGesture::delShm()
{
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_only, config.shmName.c_str());

    //When done, destroy gesture from the segment
    segment.destroy<int>(config.shmGestureName.c_str());

    //When done, destroy bbCenter from the segment
    segment.destroy<Landmark>(config.shmbbCenter.c_str());

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
    gesture = segment.find<int>(
        config.shmGestureName.c_str()).first;

    // Construct an variable in shm
    bbCenter = segment.find<Landmark>(
        config.shmbbCenter.c_str()).first;
}
void HandGesture::HandGesture::initLandmark()
{
    landmarks = new Landmark*[config.handNum];
    for(int i=0; i<config.handNum; i++){
        landmarks[i] = new Landmark[config.jointNum];
    }
}
}