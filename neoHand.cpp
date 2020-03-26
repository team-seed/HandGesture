#include "HandGesture.hpp"

int main()
{
    // NeoHand part
    HandGesture::HandGesture hg;
    // after HandGesture setGesture()
    hg.recvGesture();

    return 0;
}