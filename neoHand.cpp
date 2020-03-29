#include "HandGesture.hpp"

int main()
{
    // NeoHand part
    HandGesture::HandGesture hg;
    hg.openShm();
    // after HandGesture setGesture()
    hg.recvGesture();

    return 0;
}