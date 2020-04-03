#include "HandGesture.hpp"
#include <boost/interprocess/managed_shared_memory.hpp>

void print(HandGesture::Gesture *ges)
{
    while(ges != 0){
        for(int hand=0; hand<HandGesture::handNum; hand++){
            std::cout << "hand: " << hand << ges[hand] << std::endl;
        }
    }
}

int main()
{
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, HandGesture::shmName, HandGesture::shmSize);

    // Construct an variable in shared memory
    HandGesture::Gesture *ges = segment.find<HandGesture::Gesture>(
        HandGesture::shmbbCenterGestureName).first;

    print(ges);

    return 0;
}