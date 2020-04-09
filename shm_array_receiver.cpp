#include "ShmConfig.hpp"

void print(ShmConfig::Gesture *ges)
{
    int cnt = 0;
    while(ges != 0){
        for(int hand=0; hand<ShmConfig::handNum; hand++){
            if(int(ges[hand].lm.x) != int(ges[hand].lm.y)){
                if(ges[hand].gesture != -1){
                    std::cout << "data conflict\n";
                    std::cout << ges[hand] << std::endl;
                }
            }
        }
        if(ges[0].gesture != -1){
            ++cnt;
        }
        if(cnt > 1000000){
            break;
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

    segment.destroy<ShmConfig::Gesture>(ShmConfig::shmbbCenterGestureName);

    return 0;
}