#include <unistd.h>
#include <iostream>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "ShmConfig.hpp"

void print(ShmConfig::Gesture *gesture)
{
    int cnt = 0;
    while(cnt < 10){
        if(!gesture){
            std::cout << "gesture failed\n";
        }
        {
            // lock start
            boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lock(gesture->mutex);
            if(!gesture->gestureUpdate){
                gesture->condEmpty.wait(lock);
            }
            
            // receive data
            std::cout << *gesture << std::endl;
            ++cnt;

            // Notify the other process that the buffer is empty
            gesture->condFull.notify_one();
            gesture->gestureUpdate = false;
            // lock end
        }
        
    }
}
int main()
{
    pid_t pid;

    pid = fork();

    if(pid == 0){
        system("cd ../.. && sh runHandTrackingGPU.sh");
    }
    else if(pid > 0){
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
            ShmConfig::shmbbCenterGestureName)();

        print(gesture);

        segment.destroy<ShmConfig::Gesture>(ShmConfig::shmbbCenterGestureName);
    }
    else{
        std::cout << "fork error\n";
    }

    return 0;
}
