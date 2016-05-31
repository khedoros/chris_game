LIBS := -lsfml-system -lsfml-audio -lsfml-window -lsfml-graphics

.PHONY: main
main: chrisgame chrisgame-g

chrisgame: main.cpp
	g++ -std=c++11 main.cpp $(LIBS) -o chrisgame

chrisgame-g: main.cpp
	g++ -g -std=c++11 main.cpp $(LIBS) -o chrisgame-g
