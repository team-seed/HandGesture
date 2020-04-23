// compile command:
//  1) For opencv 4: g++ testCam.cpp -o testCam.o `pkg-config --cflags --libs opencv4`
//  2) For opencv above 4: g++ testCam.cpp -o testCam.o `pkg-config --cflags --libs opencv`
// run command:
//  ./testCam.o
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

void printCamInfo(const cv::VideoCapture &capture)
{
    std::cout << "fourcc: " << capture.get(cv::CAP_PROP_FOURCC) << std::endl;
    std::cout << "width: " << capture.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "height: " << capture.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "fps: " << capture.get(cv::CAP_PROP_FPS) << std::endl;
}

int main()
{
    auto camID{0};
    std::cout << "input your cam ID: \n";
    std::cin >> camID;
    cv::VideoCapture capture(camID);
    capture.open(camID);
    if (!capture.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "Camera opened, your default setting\n";
    printCamInfo(capture);
    
    auto camWidth{0}, camHeight{0}, fps{0};
    std::cout << "input your cam width, height and fps separated by space respectively\n";
    std::cin >> camWidth >> camHeight >> fps;

    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    capture.set(cv::CAP_PROP_FOURCC, fourcc);
    capture.set(cv::CAP_PROP_FRAME_WIDTH, camWidth);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, camHeight);
    capture.set(cv::CAP_PROP_FPS, fps);
	
    std::cout << "After setting\n";
    printCamInfo(capture);

    return 0;
}