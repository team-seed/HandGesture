#include <unistd.h>
#include <iostream>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "ShmConfig.hpp"

void print(ShmConfig::Gesture *gesture)
{
    int cnt = 0;
    while(cnt < 100000){
        if(!gesture){
            std::cout << "gesture failed\n";
        }
            
        // receive data
        std::cout << *gesture << std::endl;
        ++cnt;
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
