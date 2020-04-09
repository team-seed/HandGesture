#include <iostream>
#include "ShmConfig.hpp"

void send(ShmConfig::ShmGestureList *gestureList, ShmConfig::ShmVoidAllocator &shmVoidAllocator)
{
    int cnt {0};
    std::cout << gestureList->max_size() << std::endl;
    while(true){
        if(gestureList->size() < 500){
            ShmConfig::ShmNormalized2DPointVector v(shmVoidAllocator);
            ShmConfig::Gesture g(shmVoidAllocator);
            v.push_back({cnt, cnt});
            v.push_back({cnt*2, cnt*2});
            g.lm = v;
            g.gesture = cnt;
            gestureList->push_back(g);
            
            std::cout << cnt << std::endl;
            cnt = cnt > 10000 ? 0 : cnt+1;
        }
        if(!gestureList){
            std::cout << "gestureList failed\n";
        }
    }
}
int main()
{
/*
    // remove shmName before usage and after usage
    struct ShmPreventer{
        ShmPreventer(){boost::interprocess::shared_memory_object::remove(ShmConfig::shmName);}
        ~ShmPreventer(){boost::interprocess::shared_memory_object::remove(ShmConfig::shmName);}
    }shmPreventer;
  
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    ShmConfig::ShmVoidAllocator shmVoidAllocator(segment.get_segment_manager());
    //ShmConfig::ShmGestureAllocator shmGestureAllocator(segment.get_segment_manager());

    // Construct an variable in shared memory
    ShmConfig::ShmGestureList *gestureList = segment.construct<ShmConfig::ShmGestureList>(
        ShmConfig::shmbbCenterGestureName)(shmVoidAllocator);
*/
    // get an array from shared memory
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    // Construct an variable in shared memory
    ShmConfig::ShmGestureList *gestureList = segment.find<ShmConfig::ShmGestureList>(
        ShmConfig::shmbbCenterGestureName).first;

    //ShmConfig::ShmNormalized2DPointAllocator shmVectorAllocator(segment.get_segment_manager());
    ShmConfig::ShmVoidAllocator shmVoidAllocator(segment.get_segment_manager());
    //ShmConfig::ShmGestureAllocator shmGestureAllocator(segment.get_segment_manager());
    
    ShmConfig::ShmNormalized2DPointVector v(shmVoidAllocator);
    ShmConfig::Gesture g(shmVoidAllocator);
    //ShmConfig::Gesture g(shmVoidAllocator);

    send(gestureList, shmVoidAllocator);

    return 0;
}
