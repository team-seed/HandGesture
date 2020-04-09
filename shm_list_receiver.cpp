#include <iostream>
#include "ShmConfig.hpp"

void print(ShmConfig::ShmGestureList *ges, ShmConfig::ShmVoidAllocator &shmVoidAllocator)
{
    while(true){
        if(!ges->empty()){
            ShmConfig::Gesture g(shmVoidAllocator);
            g = boost::move(ges->front());
            ges->pop_front();

            std::cout << g << std::endl;
        }
        if(!ges){
            std::cout << "ges failed\n";
        }
    }
}
/*
void createShm()
{
    // remove shmName before usage and after usage
    struct ShmPreventer{
        ShmPreventer(){boost::interprocess::shared_memory_object::remove(ShmConfig::shmName);}
        ~ShmPreventer(){boost::interprocess::shared_memory_object::remove(ShmConfig::shmName);}
    }shmPreventer;
  
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    ShmConfig::ShmGestureAllocator shmGestureAllocator(segment.get_segment_manager());

    // Construct an variable in shared memory
    ShmConfig::ShmGestureList *gestureList = segment.construct<ShmConfig::ShmGestureList>(
        ShmConfig::shmbbCenterGestureName)();

    // use gestureList in this function

    // destroy gestureList when finished
    segment.destroy<ShmConfig::ShmGestureList>(ShmConfig::shmbbCenterGestureName);
}
void openShm()
{
    // remove shmName before usage and after usage
    struct ShmPreventer{
        ShmPreventer(){boost::interprocess::shared_memory_object::remove(ShmConfig::shmName);}
        ~ShmPreventer(){boost::interprocess::shared_memory_object::remove(ShmConfig::shmName);}
    }shmPreventer;
  
    // Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, ShmConfig::shmName, ShmConfig::shmSize);

    ShmConfig::ShmGestureAllocator shmGestureAllocator(segment.get_segment_manager());

    // Construct an variable in shared memory
    ShmConfig::ShmGestureList *gestureList = segment.construct<ShmConfig::ShmGestureList>(
        ShmConfig::shmbbCenterGestureName)();
}*/
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

    ShmConfig::ShmVoidAllocator shmVoidAllocator(segment.get_segment_manager());
    //ShmConfig::ShmGestureAllocator shmGestureAllocator(segment.get_segment_manager());

    // Construct an variable in shared memory
    ShmConfig::ShmGestureList *gestureList = segment.construct<ShmConfig::ShmGestureList>(
        ShmConfig::shmbbCenterGestureName)(shmVoidAllocator);

/*
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
*/
    print(gestureList, shmVoidAllocator);

    segment.destroy<ShmConfig::ShmGestureList>(ShmConfig::shmbbCenterGestureName);

    return 0;
}
