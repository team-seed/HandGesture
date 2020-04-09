#include "ShmConfig.hpp"
#include <boost/interprocess/managed_shared_memory.hpp>
int cnt=0;
void put()
{
    while(true){
        // get an array from shared memory
        // Create a new segment with given name and size
        boost::interprocess::managed_shared_memory segment(
            boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

        // Construct an variable in shared memory
        ShmConfig::Gesture *ges = segment.find<ShmConfig::Gesture>(
            ShmConfig::shmbbCenterGestureName).first;
        if(!ges){
            break;
        }
        for(int hand=0; hand<ShmConfig::handNum; hand++){
            ges[hand] = {{cnt, cnt, cnt} , cnt};
            std::cout << "hand: " << hand << ges[hand] << std::endl;
        }
        cnt = cnt > 100000 ? 0 : cnt + 1;
    }
}

int main()
{
    put();

    return 0;
}