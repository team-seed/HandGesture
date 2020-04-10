#include "ShmConfig.hpp"

namespace ShmConfig{
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
        return o << " (x, y, gesture) = " << n2d.x << ", " << n2d.y << " " << n2d.gesture;
    }
    std::ostream& operator<<(std::ostream& o, const Gesture &g)
    {
        o << "handNum: " << g.outputHandNum << std::endl;
        for(int hand=0; hand<g.outputHandNum; hand++){
            o << g.lm[hand] << std::endl;
        }
        return o;
    }
}