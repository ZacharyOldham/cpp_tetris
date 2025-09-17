#include <iostream>
#include <vector>
#include <sstream>
#include "tetris/piece.h"

// Include test files to get shape definitions and helper functions
// Note: This works now because we removed the main functions from these files
#include "test_piece.cpp"
#include "test_pieces.cpp"


void validateShapeTransformations(const std::vector<Shape> &shapes, const std::string &shape_name)
{
    std::cout << "Validating " << shape_name << " transformations:\n";

    // Test each transformation from the base shape (index 0)
    const Shape &base_shape = shapes[0];

    // Test 90° CW rotation (should match shapes[1])
    {
        TetrisPiece computed{base_shape};
        computed.rotateClockwise();
        TetrisPiece hardcoded{shapes[1]};

        if (!piecesEqual(computed, hardcoded))
        {
            std::cout << "ERROR: 90° CW rotation mismatch in " << shape_name << "\n";
            std::cout << "Hardcoded (shapes[1]):\n" << hardcoded << "\n";
            std::cout << "Computed (base + rotateClockwise()):\n" << computed << "\n";

            throw std::runtime_error("Validation failed");
        }
        else
        {
            std::cout << "✓ 90° CW rotation correct\n";
        }
    }

    // Test 180° rotation (should match shapes[2])
    {
        TetrisPiece computed{base_shape};
        computed.rotate180();
        TetrisPiece hardcoded{shapes[2]};

        if (!piecesEqual(computed, hardcoded))
        {
            std::cout << "ERROR: 180° rotation mismatch in " << shape_name << "\n";

            // Show internal grid representation
            std::cout << "Hardcoded (shapes[2]):\n" << hardcoded << "\n";
            std::cout << "Computed (base + rotate180()):\n" << computed << "\n";
            throw std::runtime_error("Validation failed");
        }
        else
        {
            std::cout << "✓ 180° rotation correct\n";
        }
    }

    // Test 270° CW rotation (should match shapes[3])
    {
        TetrisPiece computed{base_shape};
        computed.rotateCounterClockwise();
        TetrisPiece hardcoded{shapes[3]};

        if (!piecesEqual(computed, hardcoded))
        {
            std::cout << "ERROR: 270° CW (CCW) rotation mismatch in " << shape_name << "\n";
            std::cout << "Hardcoded (shapes[3]):\n" << hardcoded << "\n";
            std::cout << "Computed (base + rotateCounterClockwise()):\n" << computed << "\n";
            throw std::runtime_error("Validation failed");
        }
        else
        {
            std::cout << "✓ 270° CW rotation correct\n";
        }
    }

    // Test horizontal flip (should match shapes[4])
    {
        TetrisPiece computed{base_shape};
        computed.flipHorizontal();
        TetrisPiece hardcoded{shapes[4]};

        if (!piecesEqual(computed, hardcoded))
        {
            std::cout << "ERROR: Horizontal flip mismatch in " << shape_name << "\n";
            std::cout << "Hardcoded (shapes[4]):\n" << hardcoded << "\n";
            std::cout << "Computed (base + flipHorizontal()):\n" << computed << "\n";
            throw std::runtime_error("Validation failed");
        }
        else
        {
            std::cout << "✓ Horizontal flip correct\n";
        }
    }

    // Test vertical flip (should match flip + 180 = shapes[6])
    {
        TetrisPiece computed{base_shape};
        computed.flipVertical();
        TetrisPiece hardcoded{shapes[6]};

        if (!piecesEqual(computed, hardcoded))
        {
            std::cout << "ERROR: Vertical flip mismatch in " << shape_name << "\n";
            std::cout << "Hardcoded (shapes[6]):\n" << hardcoded << "\n";
            std::cout << "Computed (base + flipVertical()):\n" << computed << "\n";
            throw std::runtime_error("Validation failed");
        }
        else
        {
            std::cout << "✓ Vertical flip correct\n";
        }
    }

    // Test compound transformations
    // Flipped + 90° CW (should match shapes[5])
    {
        TetrisPiece computed{base_shape};
        computed.flipHorizontal();
        computed.rotateClockwise();
        TetrisPiece hardcoded{shapes[5]};

        if (!piecesEqual(computed, hardcoded))
        {
            std::cout << "ERROR: Flipped + 90° CW mismatch in " << shape_name << "\n";
            std::cout << "Hardcoded (shapes[5]):\n" << hardcoded << "\n";
            std::cout << "Computed (base + flipHorizontal() + rotateClockwise()):\n" << computed << "\n";
            throw std::runtime_error("Validation failed");
        }
        else
        {
            std::cout << "✓ Flipped + 90° CW correct\n";
        }
    }

    // Flipped + 270° CW (should match shapes[7])
    {
        TetrisPiece computed{base_shape};
        computed.flipHorizontal();
        computed.rotateCounterClockwise();
        TetrisPiece hardcoded{shapes[7]};

        if (!piecesEqual(computed, hardcoded))
        {
            std::cout << "ERROR: Flipped + 270° CW mismatch in " << shape_name << "\n";
            std::cout << "Hardcoded (shapes[7]):\n" << hardcoded << "\n";
            std::cout << "Computed (base + flipHorizontal() + rotateCounterClockwise()):\n" << computed << "\n";
            throw std::runtime_error("Validation failed");
        }
        else
        {
            std::cout << "✓ Flipped + 270° CW correct\n";
        }
    }

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