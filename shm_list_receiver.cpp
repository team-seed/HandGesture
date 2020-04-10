#include <iostream>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "ShmConfig.hpp"

void print(ShmConfig::Gesture *gestureList)
{
    while(true){
        if(!gestureList){
            std::cout << "gestureList failed\n";
        }
        {
            // lock start
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(gestureList->mutex);
            if(!gestureList->gestureUpdate){
                gestureList->condEmpty.wait(lock);
            }
            
            // receive data
            std::cout << *gestureList << std::endl;

            // Notify the other process that the buffer is empty
            gestureList->condFull.notify_one();
            gestureList->gestureUpdate = false;
            // lock end
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
    ShmConfig::Gesture *gestureList = segment.construct<ShmConfig::Gesture>(
        ShmConfig::shmbbCenterGestureName)();

    print(gestureList);

    segment.destroy<ShmConfig::Gesture>(ShmConfig::shmbbCenterGestureName);

    return 0;
}
