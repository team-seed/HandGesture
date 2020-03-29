#include "HandGesture.hpp"

namespace HandGesture{
void HandGesture::HandGesture::LandmarkToGesture::landmarkToGesture(HandGesture *hg)
{
    cout << "* landmarkToGesture()" << endl;
    
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        hg->config.shmName.c_str(), hg->config.shmSize);

    // Construct an variable in shared memory
    hg->gesture = segment.find<int>(
        hg->config.shmGestureName.c_str()).first;

    // Construct an variable in shm
    hg->bbCenter = segment.find<Landmark>(
        hg->config.shmbbCenter.c_str()).first;

    cout << "receive bbCenter" << *(hg->bbCenter) << endl;

    *(hg->gesture) = 1;

    cout << "send gesture " << *(hg->gesture) << endl;
}
}