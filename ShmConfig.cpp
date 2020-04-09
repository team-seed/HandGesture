#include "ShmConfig.hpp"

namespace ShmConfig{
    const int handNum = 2;
    const int jointNum = 21;
    // shm settings
    const char *shmName = "HandGesture";
    const int shmSize = 65536;
    const char *shmbbCenterGestureName = "bbCenterGesture";

    std::ostream& operator<<(std::ostream& o, const Landmark &l)
    {
        return o << " (x, y, z) = " << l.x << ", " << l.y << ", " << l.z << " " << l.angle << " ";
    }
    std::istream& operator>>(std::istream& i, Landmark &l)
    {
        return i >> l.x >> l.y >> l.z;
    }
    std::ostream& operator<<(std::ostream& o, const Normalized2DPoint &n2d)
    {
        return o << " (x, y) = " << n2d.x << ", " << n2d.y << " ";
    }
    std::ostream& operator<<(std::ostream& o, const Gesture &g)
    {
        return o << "handNum: " << g.handNum << " Gesture: " << g.gesture << " " << g.h1 << " " << g.h2 << std::endl;
    }
}