#ifndef SHMCONFIG_HPP
#define SHMCONFIG_HPP

#include <iostream>
#include <string>
#include <cmath>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

namespace ShmConfig{
    #define FLOAT_MIN 1e-10
    // modify these value in mediapipe if changed handNum
    //   * max_vec_size in 
    //     ClipVectorSizeCalculatorOptions in 
    //     multi_hand_detection_gpu.pbtxt
    //   * modify min_size in
    //     CollectionHsMinSizeCalculatorOptions in
    //     mediapipe/graphs/hand_tracking/multi_hand_tracking_mobile.pbtxt and
    //     mediapipe/graphs/hand_tracking/multi_hand_tracking_desktop_live.pbtxt.
    extern const int handNum;
    extern const int jointNum;
    // shm settings
    extern const char *shmName;
    extern const int shmSize;
    extern const char *shmbbCenterGestureName;
    struct Landmark{
        float x, y, z, angle;

        Landmark(float _x = -0.f, float _y = -0.f,float _z = -0.f, float _angle = 0.f)
        :x(_x), y(_y), z(_z), angle(_angle){}

        bool operator==(const Landmark p)
        {
            return (x==p.x && y==p.y && z==p.z);
        }

        Landmark& operator=(const Landmark &p)
        {
            x = p.x; y = p.y; z = p.z;
            return *this;
        }

        float distance(void) const
        {
            float total = 0.f;
            if(fabs(x) > FLOAT_MIN)
                total += x*x;
            if(fabs(y) > FLOAT_MIN)
                total += y*y;
            return (sqrtf(total));
        }

        float distance3d(void) const
        {
            float total = 0.f;
            if(fabs(x) > FLOAT_MIN)
                total += x*x;
            if(fabs(y) > FLOAT_MIN)
                total += y*y;
            if(fabs(z) > FLOAT_MIN)
                total += z*z;
            return (sqrtf(total));
        }

        Landmark operator+(const Landmark &p) const
        {
            return Landmark(x+p.x, y+p.y, z+p.z);
        }

        Landmark operator-(const Landmark &p) const
        {
            return Landmark(x-p.x, y-p.y, z-p.z);
        }

        // inner product
        Landmark operator*(const Landmark &p) const
        {
            return Landmark(x*p.x, y*p.y, z*p.z);
        }

        // scalar multiplication
        Landmark operator*(const float c) const
        {
            return Landmark(x*c, y*c, z*c);
        }

        void cosAngle(Landmark &a, Landmark &c)
        {
            const Landmark x = a - *this;
            const Landmark y = c - *this;
            angle = acosf((x*y).distance3d() / (x.distance3d() * y.distance3d()));
        }
    };
    std::ostream& operator<<(std::ostream& o, const Landmark &l);
    std::istream& operator>>(std::istream& i, Landmark &l);
    
    struct Normalized2DPoint{
        float x, y;

        Normalized2DPoint(float _x = 0, float _y = 0)
        :x(_x), y(_y){}
    };
    std::ostream& operator<<(std::ostream& o, const Normalized2DPoint &n2d);

    struct Gesture{
        Normalized2DPoint h1, h2;
        int gesture;
        int handNum;

        //Mutex to protect access to the queue
        boost::interprocess::interprocess_mutex      mutex;

        //Condition to wait when the queue is empty
        boost::interprocess::interprocess_condition  condEmpty;

        //Condition to wait when the queue is full
        boost::interprocess::interprocess_condition  condFull;

        //Is there any message
        bool gestureUpdate;

        Gesture(int _gesture = -1, int _handNum = -1)
        :gesture(_gesture), handNum(_handNum), gestureUpdate(false){}
    };
    std::ostream& operator<<(std::ostream& o, const Gesture &g);
}
#endif