g++ HandGesture.cpp -c -o HandGesture.o
g++ LandmarkToGesture.cpp -c -o LandmarkToGesture.o
g++ PicToLandmark.cpp -c -o PicToLandmark.o
g++ main.cpp HandGesture.o PicToLandmark.o LandmarkToGesture.o -lrt -lpthread -o main.o
g++ neoHand.cpp HandGesture.o PicToLandmark.o LandmarkToGesture.o -lrt -lpthread -o neoHand.o