CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

draug: draug.o
	g++ draug.o -o draug $(CFLAGS) $(LIBS) -std=c++11 -lboost_filesystem -lboost_system

draug.o: draug.cpp
	g++ -c draug.cpp $(CFLAGS) $(LIBS) -std=c++11 -lboost_filesystem -lboost_system

clean:
	rm *.o draug
