#include <iostream>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "ShmConfig.hpp"

void send(ShmConfig::Gesture *gesture)
{
    int cnt {0};

    while(true){
        if(!gesture){
            std::cout << "gesture failed\n";
        }
        {
            // lock start
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(gesture->mutex);
            if(gesture->gestureUpdate){
                gesture->condFull.wait(lock);
            }
            
            // send data
            for(auto &lm : gesture->lm){
                lm = {cnt, cnt, cnt};
            }
            gesture->outputHandNum = 2;

            // Notify to the other process that there is a message
            gesture->condEmpty.notify_one();

            gesture->gestureUpdate = true;
            // lock end
        }
        std::cout << cnt << std::endl;
        cnt = cnt > 10000 ? 0 : cnt+1;
    }
}
int main()
{
    // get an array from shared memory
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    // Construct an variable in shared memory
    ShmConfig::Gesture *gesture = segment.find<ShmConfig::Gesture>(
        ShmConfig::shmbbCenterGestureName).first;
    
    send(gesture);

    return 0;
}
