#include "HandGesture.hpp"

int main()
{
    // HandGesture part
    HandGesture::HandGesture hg;
    hg.getHandGestureConfig();  // file IO
    hg.p2l.picToLandmark(&hg);
    hg.l2g.landmarkToGesture(&hg);
    hg.setGesture();

    char c; cin >> c;

    return 0;
}