#ifndef HANDGESTURE_HPP
#define HANDGESTURE_HPP

#include <iostream>
#include <string>
#include <cmath>

#define FLOAT_MIN 1e-10

namespace HandGesture{
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

    struct Gesture{
        Landmark lm;
        int gesture;

        Gesture(Landmark _lm = {-0.f, -0.f, -0.f}, int _gesture = -1)
        :lm(_lm), gesture(_gesture){}
    };
    std::ostream& operator<<(std::ostream& o, const Gesture &g);

    class HandGesture{
    public:
            void landmarkToGesture();
            void angleSimilarity(int *gesReturn);
            void resize(Landmark **lm, const int &idxNum);
            void preprocess(Landmark **lm, const int &idxNum);

            void initCmpAngleArr();
            void initJointAngle(Landmark **lm, const int &idxNum);
            void initImageSize();
            void initGestureDef();
            void initGestureName();

            int *cmpAngleArr;
            int cmpAngleArrNum = 15;
            Landmark **gestureDef;
            Landmark imageSize;
            std::string *gestureName;

            int fps = 60;
            int gestureNum = 10;
            float angleSimilarityThreshold = 2.0f;
            int camID = 0;
            std::string gesturePath = "../mediapipe/mediapipe/HandGesture/store_gesture";

        // provide these four functions API for NeoHand
        //void handGestureConfig();  // NeoHand related config, implement by file IO
        void recvGesture();
        void recvPosition();

        // provide these API to get and set variables which must consider IPC issues
        void getGesture();   // aka recvGesture
        void setGesture();
        void getbbCenter();  // aka recvPosition
        void setbbCenter();
        void getHandGestureConfig();  // get all config from NeoHand and HandGesture
        void setHandGestureConfig();  // set all config from NeoHand and HandGesture

        // init shm
        //void initShm();
        //void delShm();
        //void openShm();

        // internal class using
        HandGesture(Gesture *gesturePtr);
        ~HandGesture();
        void initLandmark();
        void initbbCenter();

    //private:
        Landmark **landmarks;
        Landmark *bbCenter;
        
        // hand number from mediapipe multi hand
        int multiHandNum;
        int multiRectNum;
        
        // these three variables must consider IPC issues
        Gesture *gesture;
    };
}

#endif