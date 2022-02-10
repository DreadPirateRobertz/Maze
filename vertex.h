//
// Created by darklord on 11/30/21.
//
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <cstdlib>


using namespace std;

#ifndef MAZE_TILE_H
#define MAZE_TILE_H


enum class Color {WHITE, GREY, BLACK, NONE};
enum class Direction{N, S, E, W, NE, NW, SE, SW, NONE};
class vertex {
private:

public:
    int row, col;
    char arrowColor, circle;
    string side;
    Direction direction;
    vector<vertex*> adjList;
    vertex *parentPtr;
    Color color;
    vertex();
};


#endif //MAZE_TILE_H
