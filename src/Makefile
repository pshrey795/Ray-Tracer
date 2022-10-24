CC=g++
CFLAGS= -fopenmp -lGL -lGLU -lglut

rtrace: main.cpp
	$(CC) -o rtrace main.cpp $(CFLAGS) 

run: rtrace
	./rtrace $(ARGS)

clean:
	rm -f *.o
	rm -f *.ppm
	rm -f rtrace