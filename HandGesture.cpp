#include "HandGesture.hpp"
/*
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
*/
namespace HandGesture{
    
const int handNum = 2;
const int jointNum = 21;
// shm settings
const char *shmName = "HandGesture";
const int shmSize = 1024;
const char *shmbbCenterGestureName = "bbCenterGesture";

ostream& operator<<(ostream& o, const Landmark &l)
{
    return o << " (x, y, z) = " << l.x << ", " << l.y << ", " << l.z << " ";
}
istream& operator>>(std::istream& i, Landmark &l)
{
    return i >> l.x >> l.y >> l.z;
}
ostream& operator<<(std::ostream& o, const Gesture &g)
{
    return o << " gesture " << g.lm << " is " << g.gesture << " ";
}
HandGesture::HandGesture::HandGesture(Gesture *gesturePtr)
:gesture(gesturePtr)
{
    cout << "* HandGesture()" << endl;
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
    segment.construct<Gesture>(shmbbCenterGestureName)[handNum]();
}
void HandGesture::HandGesture::delShm()
{
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_only, shmName);

    //When done, destroy gesture from the segment
    segment.destroy<Gesture>(shmbbCenterGestureName);

    boost::interprocess::shared_memory_object::remove(shmName);
}
void HandGesture::HandGesture::openShm()
{
    cout << "* openShm" << endl;

    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, shmName, shmSize);

    // Construct an variable in shared memory
    segment.find<Gesture>(
        shmbbCenterGestureName).first;
}
*/
void HandGesture::HandGesture::initLandmark()
{
    cout << "* initLandmark()\n";
    landmarks = new Landmark*[handNum];
    for(int i=0; i<handNum; i++){
        landmarks[i] = new Landmark[jointNum];
    }
}
void HandGesture::HandGesture::getHandGestureConfig()
{
    
}
void HandGesture::HandGesture::initbbCenter()
{
    cout << "* initbbCenter()\n";
    bbCenter = new Landmark[handNum];
}
}