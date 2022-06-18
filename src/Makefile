
output: main.o readconf.o logging.o
	g++ main.o readconf.o logging.o -lX11 -o ../p3wm.x11

logging.o: logging.cpp
	g++ -c logging.cpp -o logging.o

readconf.o: readconf.cpp
	g++ -c readconf.cpp -o readconf.o

main.o: main.cpp
	g++ -c main.cpp -Wno-deprecated-declarations

clean:
	rm *.o ../p3wm.x11