#!/bin/bash
g++ tetris.cpp -I/usr/local/Cellar/sfml/2.5.1_1  -o tetris -L/usr/local/Cellar/sfml/2.5.1_1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
./tetris 