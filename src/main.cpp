#include <iostream>
#include <vector>
#include "tetris/piece.h"

int main()
{
    TetrisPiece piece = TetrisPiece{{{false, true, false, true, false, true},
                                     {true, false, true, false, true, false},
                                     {false, true, false, true, false, true},
                                     {true, false, true, false, true, false},
                                     {true, true, true, true, true, true}}};
    std::cout << piece;
    piece.rotateClockwise();
    std::cout << piece;
    TetrisPiece piece2 = TetrisPiece{
        {{true, true, false, true, false},
         {true, false, true, false, true},
         {true, true, false, true, false},
         {true, false, true, false, true},
         {true, true, false, true, false},
         {true, false, true, true, true}},
    };
    std::cout << piece2;
}