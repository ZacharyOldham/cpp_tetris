#include <iostream>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <limits>
#include "tetris/piece.h"

// Helper function to compare two pieces by their string representation
bool piecesEqual(const TetrisPiece& a, const TetrisPiece& b) {
    std::stringstream ss_a, ss_b;
    ss_a << a;
    ss_b << b;
    return ss_a.str() == ss_b.str();
}

void test_constructor_valid() {
    TetrisPiece piece{{{true, false}, {false, true}}};
    std::cout << "✓ Valid constructor test passed\n";
}

void test_constructor_empty_width() {
    try {
        TetrisPiece piece{{}};
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        std::cout << "✓ Empty width constructor test passed\n";
    }
}

void test_constructor_zero_height() {
    try {
        TetrisPiece piece{{{}, {}}};
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        std::cout << "✓ Zero height constructor test passed\n";
    }
}

void test_constructor_non_uniform_height() {
    try {
        TetrisPiece piece{{{true}, {true, false}}};
        assert(false); // Should not reach here
    } catch (const std::invalid_argument&) {
        std::cout << "✓ Non-uniform height constructor test passed\n";
    }
}

void test_factory_functions() {
    TetrisPiece q_piece = TetrisPiece::createQPiece();
    TetrisPiece z_piece = TetrisPiece::createZPiece();
    TetrisPiece t_piece = TetrisPiece::createTPiece();
    TetrisPiece i_piece = TetrisPiece::createIPiece();
    TetrisPiece l_piece = TetrisPiece::createLPiece();

    // Verify they create different pieces
    assert(!piecesEqual(q_piece, z_piece));
    assert(!piecesEqual(q_piece, t_piece));

    std::cout << "✓ Factory functions test passed\n";
}

void test_factory_map_access() {
    TetrisPiece piece1 = TetrisPiece::createQPiece();
    TetrisPiece piece2 = TetrisPiece::pieceFactories.at('Q')();

    assert(piecesEqual(piece1, piece2));
    std::cout << "✓ Factory map access test passed\n";
}

void test_flip_horizontal() {
    // Use L-piece: asymmetric shape
    // Original L-piece: {{true, true, true}, {true, false, false}}
    TetrisPiece piece = TetrisPiece::createLPiece();
    piece.flipHorizontal();

    // After horizontal flip: {{true, false, false}, {true, true, true}}
    TetrisPiece expected{{{true, false, false}, {true, true, true}}};

    assert(piecesEqual(piece, expected));
    std::cout << "✓ Flip horizontal test passed\n";
}

void test_flip_vertical() {
    // Use L-piece: {{true, true, true}, {true, false, false}}
    TetrisPiece piece = TetrisPiece::createLPiece();
    piece.flipVertical();

    // After vertical flip: {{true, true, true}, {false, false, true}}
    TetrisPiece expected{{{true, true, true}, {false, false, true}}};

    assert(piecesEqual(piece, expected));
    std::cout << "✓ Flip vertical test passed\n";
}

void test_rotate_clockwise() {
    // Use L-piece for asymmetry
    // Original: {{true, true, true}, {true, false, false}}
    TetrisPiece piece = TetrisPiece::createLPiece();
    piece.rotateClockwise();

    // 90° CW rotation of L-piece: {{true, true}, {false, true}, {false, true}}
    TetrisPiece expected{{{true, true}, {false, true}, {false, true}}};

    assert(piecesEqual(piece, expected));
    std::cout << "✓ Rotate clockwise test passed\n";
}

void test_rotate_counter_clockwise() {
    // Use L-piece for asymmetry
    // Original: {{true, true, true}, {true, false, false}}
    TetrisPiece piece = TetrisPiece::createLPiece();
    piece.rotateCounterClockwise();

    // 90° CCW rotation of L-piece: {{true, false}, {true, false}, {true, true}}
    TetrisPiece expected{{{true, false}, {true, false}, {true, true}}};

    assert(piecesEqual(piece, expected));
    std::cout << "✓ Rotate counter-clockwise test passed\n";
}

void test_rotate_180() {
    // Use L-piece for asymmetry
    // Original: {{true, true, true}, {true, false, false}}
    TetrisPiece piece = TetrisPiece::createLPiece();
    piece.rotate180();

    // 180° rotation of L-piece: {{false, false, true}, {true, true, true}}
    TetrisPiece expected{{{false, false, true}, {true, true, true}}};

    assert(piecesEqual(piece, expected));
    std::cout << "✓ Rotate 180 test passed\n";
}

void test_multiple_transformations() {
    // Chain operations and verify final result
    TetrisPiece piece = TetrisPiece::createLPiece();

    // Rotate 90° CW twice should equal 180° rotation
    piece.rotateClockwise();
    piece.rotateClockwise();

    TetrisPiece expected = TetrisPiece::createLPiece();
    expected.rotate180();

    assert(piecesEqual(piece, expected));
    std::cout << "✓ Multiple transformations test passed\n";
}

void test_single_block_piece() {
    TetrisPiece piece{{{true}}};

    // All transformations should leave single block unchanged
    piece.flipHorizontal();
    piece.flipVertical();
    piece.rotateClockwise();
    piece.rotate180();

    TetrisPiece expected{{{true}}};
    assert(piecesEqual(piece, expected));
    std::cout << "✓ Single block piece test passed\n";
}

void test_single_column_piece() {
    TetrisPiece piece{{{true, false, true}}};

    // Test that transformations work on tall thin pieces
    piece.flipVertical();
    TetrisPiece expected{{{true, false, true}}};
    assert(piecesEqual(piece, expected));
    std::cout << "✓ Single column piece test passed\n";
}

void test_single_row_piece() {
    TetrisPiece piece{{{true}, {false}, {true}}};

    // Test that transformations work on wide flat pieces
    piece.flipHorizontal();
    TetrisPiece expected{{{true}, {false}, {true}}};
    assert(piecesEqual(piece, expected));
    std::cout << "✓ Single row piece test passed\n";
}

void test_lowest_block_normal() {
    TetrisPiece piece{{{false, true, false}, {true, false, true}}};

    assert(piece.lowestBlockInColumn(0) == 1); // First true in column 0 is at row 1
    assert(piece.lowestBlockInColumn(1) == 0); // First true in column 1 is at row 0
    std::cout << "✓ Lowest block normal test passed\n";
}

void test_lowest_block_empty_column() {
    TetrisPiece piece{{{true, false}, {false, false}}};

    assert(piece.lowestBlockInColumn(1) == std::numeric_limits<size_t>::max());
    std::cout << "✓ Lowest block empty column test passed\n";
}

void test_string_representation() {
    TetrisPiece piece{{{true}}};
    std::stringstream ss;
    ss << piece;

    std::string output = ss.str();
    // Should contain borders and the X
    assert(output.find("---") != std::string::npos);
    assert(output.find("|X|") != std::string::npos);
    std::cout << "✓ String representation test passed\n";
}

void runBasicTetrisPieceTests() {
    std::cout << "Running Basic TetrisPiece Tests...\n";

    // Constructor tests
    test_constructor_valid();
    test_constructor_empty_width();
    test_constructor_zero_height();
    test_constructor_non_uniform_height();

    // Factory tests
    test_factory_functions();
    test_factory_map_access();

    // Transformation tests
    test_flip_horizontal();
    test_flip_vertical();
    test_rotate_clockwise();
    test_rotate_counter_clockwise();
    test_rotate_180();
    test_multiple_transformations();

    // Edge case tests
    test_single_block_piece();
    test_single_column_piece();
    test_single_row_piece();

    // Utility function tests
    test_lowest_block_normal();
    test_lowest_block_empty_column();

    // Output test
    test_string_representation();

    std::cout << "✓ All basic TetrisPiece tests passed!\n";
}