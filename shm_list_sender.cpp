#include <iostream>
#include "ShmConfig.hpp"

void send()
{
    int cnt {0};

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

        gestureList->h1 = {cnt, cnt};
        gestureList->h2 = {cnt*2, cnt*2};
        gestureList->gesture = cnt;
        gestureList->handNum = 2;
        
        std::cout << cnt << std::endl;
        cnt = cnt > 10000 ? 0 : cnt+1;
    }
}
int main()
{
    send();

    return 0;
}
