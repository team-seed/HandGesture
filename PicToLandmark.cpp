#include "HandGesture.hpp"

namespace HandGesture{
void HandGesture::HandGesture::PicToLandmark::picToLandmark(HandGesture *hg)
{
    cout << "* picToLandmark()" << endl;
    
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        hg->config.shmName.c_str(), hg->config.shmSize);

    // Construct an variable in shm
    hg->bbCenter = segment.find<Landmark>(
        hg->config.shmbbCenter.c_str()).first;

    *(hg->bbCenter) = {0.1f, 0.1f, 0.1f};

    cout << "send bbCenter" << *(hg->bbCenter) << endl;
}
}