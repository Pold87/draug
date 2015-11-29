CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
ALLFLAGS = $(CFLAGS) $(LIBS) -std=c++11 -lboost_filesystem -lboost_system

all: draug draug_for_folder

draug_for_folder: draug_for_folder.o draug_helpers.o
	g++ -g draug_for_folder.o draug_helpers.o -o draug_for_folder $(ALLFLAGS)

draug: draug.o draug_helpers.o
	g++ -g draug.o draug_helpers.o -o draug $(ALLFLAGS)

draug_for_folder.o: draug_for_folder.cpp
	g++ -g -c draug_for_folder.cpp $(ALLFLAGS)

draug.o: draug_helpers.cpp
	g++ -g -c draug.cpp $(ALLFLAGS)

draug_helpers.o: draug_helpers.cpp draug_helpers.h	
	g++ -g -c draug_helpers.cpp $(ALLFLAGS)

clean:
	rm *.o draug draug_for_folder
