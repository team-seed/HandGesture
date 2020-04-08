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
    // remove shmName before usage and after usage
    struct ShmPreventer{
        ShmPreventer(){boost::interprocess::shared_memory_object::remove(ShmConfig::shmName);}
        ~ShmPreventer(){boost::interprocess::shared_memory_object::remove(ShmConfig::shmName);}
    }shmPreventer;
  
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    // Construct an variable in shared memory
    ShmConfig::Gesture *gesture = segment.construct<ShmConfig::Gesture>(
        ShmConfig::shmbbCenterGestureName)[ShmConfig::handNum]();
    
/*
    // get an array from shared memory
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    // Construct an variable in shared memory
    ShmConfig::Gesture *ges = segment.find<ShmConfig::Gesture>(
        ShmConfig::shmbbCenterGestureName).first;
*/
    print(gesture);

    return 0;
}