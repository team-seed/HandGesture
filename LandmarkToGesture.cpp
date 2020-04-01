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
    Gesture *gesture = segment.find<Gesture>(
        hg->config.shmbbCenterGestureName.c_str()).first;

    
}
}