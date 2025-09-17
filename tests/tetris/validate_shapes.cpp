#include <iostream>
#include <vector>
#include <sstream>
#include <functional>
#include "tetris/piece.h"

// Include test files to get shape definitions and helper functions
// Note: This works now because we removed the main functions from these files
#include "test_piece.cpp"
#include "test_pieces.cpp"

void validateTransformation(
    const Shape &base_shape,
    const Shape &expected_shape,
    std::function<void(TetrisPiece&)> transformation,
    const std::string &test_name,
    const std::string &shape_name)
{
    TetrisPiece computed{base_shape};
    transformation(computed);
    TetrisPiece hardcoded{expected_shape};

    if (!piecesEqual(computed, hardcoded))
    {
        std::cout << "ERROR: " << test_name << " mismatch in " << shape_name << "\n";
        std::cout << "Hardcoded:\n" << hardcoded << "\n";
        std::cout << "Computed:\n" << computed << "\n";
        throw std::runtime_error("Validation failed");
    }
    else
    {
        std::cout << "✓ " << test_name << " correct\n";
    }
}

void validateShapeTransformations(const std::vector<Shape> &shapes, const std::string &shape_name)
{
    std::cout << "Validating " << shape_name << " transformations:\n";

    const Shape &base_shape = shapes[0];

    // Test single transformations
    validateTransformation(base_shape, shapes[1],
        [](TetrisPiece& p) { p.rotateClockwise(); },
        "90° CW rotation", shape_name);

    validateTransformation(base_shape, shapes[2],
        [](TetrisPiece& p) { p.rotate180(); },
        "180° rotation", shape_name);

    validateTransformation(base_shape, shapes[3],
        [](TetrisPiece& p) { p.rotateCounterClockwise(); },
        "270° CW rotation", shape_name);

    validateTransformation(base_shape, shapes[4],
        [](TetrisPiece& p) { p.flipHorizontal(); },
        "Horizontal flip", shape_name);

    validateTransformation(base_shape, shapes[6],
        [](TetrisPiece& p) { p.flipVertical(); },
        "Vertical flip", shape_name);

    // Test compound transformations
    validateTransformation(base_shape, shapes[5],
        [](TetrisPiece& p) { p.flipHorizontal(); p.rotateClockwise(); },
        "Flipped + 90° CW", shape_name);

    validateTransformation(base_shape, shapes[7],
        [](TetrisPiece& p) { p.flipHorizontal(); p.rotateCounterClockwise(); },
        "Flipped + 270° CW", shape_name);

    std::cout << "\n";
}

int main()
{
    std::cout << "Shape Transformation Validation Tool\n";
    std::cout << "====================================\n\n";

    // Validate each shape group using the existing shape vectors
    try
    {
        validateShapeTransformations(shape_5x5, "5x5");
        validateShapeTransformations(shape_5x6, "5x6");
        validateShapeTransformations(shape_6x5, "6x5");
        validateShapeTransformations(shape_1x5, "1x5");
        validateShapeTransformations(shape_1x6, "1x6");
        validateShapeTransformations(shape_5x1, "5x1");
        validateShapeTransformations(shape_6x1, "6x1");
    }
    catch (const std::runtime_error &)
    {
        std::cout << "Validation failed!\n";
        return 1;
    }

    std::cout << "Validation complete!\n";
    return 0;
}