CC = g++
CFLAGS = -std=c++17 -O3 -Wl,-rpath,/usr/local/lib/
# LIBS = -L/home/kyle/Actor_Container_Fun/actor_install/lib -lcaf_core
LIBS = -lcaf_core -lcaf_io
# INCLUDES = -I/home/kyle/Actor_Container_Fun/actor_install/include
INCLUDES = 

all: server client

server: server.cpp common.hpp
	$(CC) $(CFLAGS) $(INCLUDES) -o server server.cpp $(LIBS)

client: client.cpp common.hpp
	$(CC) $(CFLAGS) $(INCLUDES) -o client client.cpp $(LIBS)
# main: main.cpp
# 	g++ $(CFLAGS) main.cpp -o main -fopenmp

# mpi: mpi_main.cpp
# 	mpic++ $(CFLAGS) $(INCLUDES) -o mpi_main mpi_main.cpp

# new_draft: newdraft.cpp
# 	$(CC) $(CFLAGS) $(INCLUDES) -o newdraft newdraft.cpp $(LIBS)

clean:
	rm -f main *.o