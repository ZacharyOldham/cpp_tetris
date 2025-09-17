#include <iostream>
#include "tetris/piece.h"

// Include test files
#include "tetris/test_piece.cpp"
#include "tetris/test_pieces.cpp"

int main() {
    std::cout << "Running All TetrisPiece Tests\n";
    std::cout << "=============================\n\n";

    // Run basic tests
    runBasicTetrisPieceTests();

    std::cout << "\n";

    // Run comprehensive transformation tests
    testAllTransformations();

    std::cout << "\n🎉 All tests completed!\n";
    return 0;
}