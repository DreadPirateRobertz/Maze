//
// Created by darklord on 11/30/21.
//
#include "vertex.h"

vertex::vertex() {
    row = -1;
    col = -1;
    circle = '\0';
    arrowColor = '\0';
    direction = Direction::NONE;
    side = "--->";
    parentPtr = nullptr;
    color = Color::NONE;
}