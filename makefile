all: Proyecto1Arqui

Proyecto1Arqui: Proyecto1Arqui.cpp Screen.h MainLoop.h
	g++ Proyecto1Arqui.cpp -lncurses -o Proyecto1Arqui