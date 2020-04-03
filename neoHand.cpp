#include "ShmConfig.hpp"
#include <boost/interprocess/managed_shared_memory.hpp>

void print(ShmConfig::Gesture *ges)
{
    while(ges != 0){
        for(int hand=0; hand<ShmConfig::handNum; hand++){
            std::cout << "hand: " << hand << ges[hand] << std::endl;
        }
    }
}

int main()
{
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    // Construct an variable in shared memory
    ShmConfig::Gesture *ges = segment.find<ShmConfig::Gesture>(
        ShmConfig::shmbbCenterGestureName).first;

    print(ges);

    return 0;
}