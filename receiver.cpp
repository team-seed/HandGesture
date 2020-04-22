// put this file in mediapipe/HandGesture folder
// compile command: 
//	1) g++ ShmConfig.cpp -c -o ShmConfig.o
//	2) g++ receiver.cpp ShmConfig.o -lrt -lpthread -o receiver
// run command: 
// 	1) ./receiver
//	2) source runHandTrackingGPU.sh
#include <iostream>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "ShmConfig.hpp"

void print(ShmConfig::Gesture *gesture)
{
    while(true){
        if(!gesture){
            std::cout << "gesture failed\n";
        }
            
        // receive data
        std::cout << *gesture << std::endl;
    }
}
int main()
{
    pid_t pid;

    pid = fork();

    if(pid == 0){
        std::cout << "child pid: " << getpid() << std::endl;
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
        exit(EXIT_FAILURE);
    }

    return 0;
}