#include "ShmConfig.hpp"

namespace ShmConfig{
    const int handNum = 2;
    const int jointNum = 21;
    // shm settings
    const char *shmName = "HandGesture";
    const int shmSize = 1024;
    const char *shmbbCenterGestureName = "bbCenterGesture";

    std::ostream& operator<<(std::ostream& o, const Landmark &l)
    {
        return o << " (x, y, z) = " << l.x << ", " << l.y << ", " << l.z << " ";
    }
    std::istream& operator>>(std::istream& i, Landmark &l)
    {
        return i >> l.x >> l.y >> l.z;
    }
    std::ostream& operator<<(std::ostream& o, const Gesture &g)
    {
        return o << " gesture " << g.lm << " is " << g.gesture << " ";
    }
}