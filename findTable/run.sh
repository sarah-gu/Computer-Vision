#!/bin/bash

g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv4.pc) -std=c++11 main.cpp && ./a.out 
