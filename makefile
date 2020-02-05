CC = g++
OBJECTS = main.o shader.o glad/glad.o planet.o universe.o camera.o
LIBS = -fopenmp -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

main: $(OBJECTS)
	$(CC) -o main $(OBJECTS) $(LIBS)
main.o: src/main.cpp
	$(CC) -std=c++11 -c src/main.cpp -Iinclude -Iglad/include
glad/glad.o: glad/src/glad.c
	$(CC) -c glad/src/glad.c -Iglad/include -o glad/glad.o
shader.o: src/shader.cpp
	$(CC) -c src/shader.cpp -Iglad/include -Iinclude
planet.o: src/planet.cpp
	$(CC) -c src/planet.cpp -Iglad/include -Iinclude
universe.o: src/universe.cpp
	$(CC) -c src/universe.cpp -Iglad/include -Iinclude
camera.o: src/camera.cpp
	$(CC) -c src/camera.cpp -Iglad/include -Iinclude
clean:
	rm -f *.o

.PHONY: clean
