#include <iostream>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "ShmConfig.hpp"

void send(ShmConfig::Gesture *gestureList)
{
    int cnt {0};

    while(true){
        if(!gestureList){
            std::cout << "gestureList failed\n";
        }
        {
            // lock start
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(gestureList->mutex);
            if(gestureList->gestureUpdate){
                gestureList->condFull.wait(lock);
            }
            
            // send data
            gestureList->h1 = {cnt, cnt};
            gestureList->h2 = {cnt*2, cnt*2};
            gestureList->gesture = cnt;
            gestureList->handNum = 2;

            // Notify to the other process that there is a message
            gestureList->condEmpty.notify_one();

            gestureList->gestureUpdate = true;
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
    ShmConfig::Gesture *gestureList = segment.find<ShmConfig::Gesture>(
        ShmConfig::shmbbCenterGestureName).first;
    
    send(gestureList);

    return 0;
}
