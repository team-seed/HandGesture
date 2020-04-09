#include <iostream>
#include "ShmConfig.hpp"

void print()
{
    while(true){
        // get an array from shared memory
        // Create a new segment with given name and size
        boost::interprocess::managed_shared_memory segment(
            boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

        // Construct an variable in shared memory
        ShmConfig::Gesture *gestureList = segment.find<ShmConfig::Gesture>(
            ShmConfig::shmbbCenterGestureName).first;
        if(!gestureList){
            std::cout << "gestureList failed\n";
        }
        std::cout << *gestureList << std::endl;
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
    ShmConfig::Gesture *gestureList = segment.construct<ShmConfig::Gesture>(
        ShmConfig::shmbbCenterGestureName)();

    print();

    segment.destroy<ShmConfig::Gesture>(ShmConfig::shmbbCenterGestureName);

    return 0;
}
