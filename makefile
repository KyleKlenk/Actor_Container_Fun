CC = g++
CFLAGS = -std=c++17 -Wl,-rpath,/usr/local/lib
LIBS = -lcaf_core
INCLUDES = 

all: actors main mpi new_draft

actors: actors_main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o actors_main actors_main.cpp $(LIBS)

main: main.cpp
	g++ main.cpp -o main -fopenmp

mpi: mpi_main.cpp
	mpic++ $(CFLAGS) $(INCLUDES) -o mpi_main mpi_main.cpp

new_draft: newdraft.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o newdraft newdraft.cpp $(LIBS)

clean:
	rm -f main actors_main *.o