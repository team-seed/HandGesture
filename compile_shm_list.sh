g++ ShmConfig.cpp -c -o ShmConfig.o
g++ shm_list_receiver.cpp ShmConfig.o -lrt -lpthread -o shm_list_receiver.o
g++ shm_list_sender.cpp ShmConfig.o -lrt -lpthread -o shm_list_sender.o