g++ ShmConfig.cpp -c -o ShmConfig.o
g++ shm_condLock_receiver.cpp ShmConfig.o -lrt -lpthread -o shm_condLock_receiver.o
g++ shm_condLock_sender.cpp ShmConfig.o -lrt -lpthread -o shm_condLock_sender.o