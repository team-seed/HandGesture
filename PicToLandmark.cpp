#include "HandGesture.hpp"

namespace HandGesture{
void HandGesture::HandGesture::PicToLandmark::picToLandmark(HandGesture *hg)
{
    cout << "* picToLandmark()" << endl;

    hg->pic = {1, 1};
    cout << "receive picture" << hg->pic << endl;
    
    hg->landmarks.push_back({0.1f, 0.1f, 0.1f});
    hg->bbCenter = {0.1f, 0.1f, 0.1f};

    cout << "send landmarks" << endl;
    for(const auto &lm : hg->landmarks){
        cout << lm << endl;
    }

    cout << "send bbCenter" << hg->bbCenter << endl;
}
}