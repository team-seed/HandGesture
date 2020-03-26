#include "HandGesture.hpp"

namespace HandGesture{
void HandGesture::HandGesture::LandmarkToGesture::landmarkToGesture(HandGesture *hg)
{
    cout << "* landmarkToGesture()" << endl;

    cout << "receive landmark" << endl;
    for(const auto &lm : hg->landmarks){
        cout << lm << endl;
    }

    hg->gesture = 1;

    cout << "send gesture " << hg->gesture << endl;
}
}