# old version for testing
#g++ HandGesture.cpp -c -o HandGesture.o
#g++ LandmarkToGesture.cpp -c -o LandmarkToGesture.o
#g++ PicToLandmark.cpp -c -o PicToLandmark.o
#g++ main.cpp HandGesture.o PicToLandmark.o LandmarkToGesture.o -lrt -lpthread -o main.o
#g++ neoHand.cpp HandGesture.o PicToLandmark.o LandmarkToGesture.o -lrt -lpthread -o neoHand.o

# for neoHand.cpp test
g++ ShmConfig.cpp -c -o ShmConfig.o
g++ shm_array_receiver.cpp ShmConfig.o -lrt -lpthread -o shm_array_receiver.o
g++ shm_array_sender.cpp ShmConfig.o -lrt -lpthread -o shm_array_sender.o