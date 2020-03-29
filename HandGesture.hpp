#ifndef HANDGESTURE_HPP
#define HANDGESTURE_HPP

#include <iostream>
#include <vector>
#include <string>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace std;

namespace HandGesture{
    struct Landmark{
        float x, y, z;

        Landmark(float _x = 0.f, float _y = 0.f,float _z = 0.f)
        :x(_x), y(_y), z(_z){}
    };
    ostream& operator<<(std::ostream& o, const Landmark &l);

    class HandGesture{
    public:
        class PicToLandmark{
        public:
            void picToLandmark(HandGesture *hg);
        };

        class LandmarkToGesture{
        public:
            void landmarkToGesture(HandGesture *hg);
        };

        class HandGestureConfig{
        public:
            // modify these value in mediapipe
            //   * max_vec_size in 
            //     ClipVectorSizeCalculatorOptions in 
            //     multi_hand_detection_gpu.pbtxt
            //   * modify min_size in
            //     CollectionHsMinSizeCalculatorOptions in
            //     mediapipe/graphs/hand_tracking/multi_hand_tracking_mobile.pbtxt and
            //     mediapipe/graphs/hand_tracking/multi_hand_tracking_desktop_live.pbtxt.
            int handNum = 2;
            int jointNum = 21;
            int fps = 60;

            // shm settings
            string shmName = "HandGesture";
            int shmSize = 1024;
            string shmbbCenter = "bbCenter";
            string shmGestureName = "Gesture";
        };

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
        void initShm();
        void delShm();
        void openShm();

        // internal class using
        HandGesture();
        //~HandGesture();
        void initLandmark();

        PicToLandmark p2l;
        LandmarkToGesture l2g;
        HandGestureConfig config;
        
    //private:
        Landmark **landmarks;
        
        // these three variables must consider IPC issues
        int *gesture;
        Landmark *bbCenter;
    };
}

#endif