CC = g++
CFLAGS = -std=c++17 -O3 -Wl,-rpath,/home/kyle/Actor_Container_Fun/actor_install/lib
# LIBS = -L/home/kyle/Actor_Container_Fun/actor_install/lib -lcaf_core
LIBS = -lcaf_core
# INCLUDES = -I/home/kyle/Actor_Container_Fun/actor_install/include
INCLUDES = 

all: actors main mpi new_draft

actors: actors_main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o actors_main actors_main.cpp $(LIBS)

main: main.cpp
	g++ $(CFLAGS) main.cpp -o main -fopenmp

mpi: mpi_main.cpp
	mpic++ $(CFLAGS) $(INCLUDES) -o mpi_main mpi_main.cpp

new_draft: newdraft.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o newdraft newdraft.cpp $(LIBS)

clean:
	rm -f main actors_main *.o