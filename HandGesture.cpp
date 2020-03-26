#include "HandGesture.hpp"

namespace HandGesture{
ostream& operator<<(ostream& o, const Picture &p)
{
    return o << " (id, content) = " << p.id << ", " << p.content << " ";
}

ostream& operator<<(ostream& o, const Landmark &l)
{
    return o << " (x, y, z) = " << l.x << ", " << l.y << ", " << l.z << " ";
}

HandGesture::HandGesture::HandGesture()
{
    cout << "* HandGesture()" << endl;
}

void HandGesture::HandGesture::recvGesture()
{
    cout << "* recvGesture()" << endl;
    cout << "receive gesture " << gesture << endl;
}
}