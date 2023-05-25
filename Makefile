LIBS := -lsfml-system -lsfml-audio -lsfml-window -lsfml-graphics

.PHONY: main
main: chrisgame chrisgame-g

chrisgame: chrisgame.cpp
	g++ -std=c++11 chrisgame.cpp $(LIBS) -o chrisgame

chrisgame-g: chrisgame.cpp
	g++ -g -std=c++11 chrisgame.cpp $(LIBS) -o chrisgame-g
