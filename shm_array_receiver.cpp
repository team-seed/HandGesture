#include "ShmConfig.hpp"

void print(ShmConfig::Gesture *ges)
{
    int cnt = 0;
    while(true){
        std::cout << *ges << std::endl;
    }
}

int main()
{
    // get an array from shared memory
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    // Construct an variable in shared memory
    ShmConfig::Gesture *ges = segment.find<ShmConfig::Gesture>(
        ShmConfig::shmbbCenterGestureName).first;

    if(ges == 0){
        std::cout << "shm open failed\n";
        exit(EXIT_FAILURE);
    }
    
    print(ges);

    segment.destroy<ShmConfig::Gesture>(ShmConfig::shmbbCenterGestureName);

    return 0;
}