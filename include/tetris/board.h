#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H

#include "piece.h"

class TetrisBoard
{
    int width{10};
    int height{30};

public:
    void addPiece(TetrisPiece piece, int col_offset);
    int maxHeight();
    int highestBlockInColumn();
};

#endif // TETRIS_BOARD_H