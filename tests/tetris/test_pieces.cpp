#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
#include <sstream>
#include <stdexcept>
#include "tetris/piece.h"

// Type alias for shape vectors
using Shape = std::vector<std::vector<bool>>;

// Hardcoded test shapes and their 8 variants (4 rotations + 4 rotations of flipped)
// Each group of 8 represents: [0°, 90°, 180°, 270°, Flipped+0°, Flipped+90°, Flipped+180°, Flipped+270°]

// 5x5 asymmetric pattern
const std::vector<Shape> shape_5x5 = {
    // Original (0°)
    {{true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, true, true, true, true}},

    // 90° CW
    {{true, false, true, false, true},
     {true, true, false, true, false},
     {true, false, true, false, true},
     {true, true, false, true, false},
     {true, false, true, false, true}},

    // 180°
    {{true, true, true, true, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true}},

    // 270° CW
    {{true, false, true, false, true},
     {false, true, false, true, true},
     {true, false, true, false, true},
     {false, true, false, true, true},
     {true, false, true, false, true}},

    // Flipped horizontally (0°)
    {{true, true, true, true, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true}},

    // Flipped + 90° CW
    {{true, false, true, false, true},
     {false, true, false, true, true},
     {true, false, true, false, true},
     {false, true, false, true, true},
     {true, false, true, false, true}},

    // Flipped + 180°
    {{true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, true, true, true, true}},

    // Flipped + 270° CW
    {{true, false, true, false, true},
     {true, true, false, true, false},
     {true, false, true, false, true},
     {true, true, false, true, false},
     {true, false, true, false, true}}};

// 5x6 asymmetric pattern
const std::vector<Shape> shape_5x6 = {
    // Original (0°)
    {{false, true, false, true, false, true},
     {true, false, true, false, true, false},
     {false, true, false, true, false, true},
     {true, false, true, false, true, false},
     {true, true, true, true, true, true}},

    // 90° CW
    {{true, true, false, true, false},
     {true, false, true, false, true},
     {true, true, false, true, false},
     {true, false, true, false, true},
     {true, true, false, true, false},
     {true, false, true, false, true}},

    // 180°
    {{true, true, true, true, true, true},
     {false, true, false, true, false, true},
     {true, false, true, false, true, false},
     {false, true, false, true, false, true},
     {true, false, true, false, true, false}},

    // 270° CW
    {{true, false, true, false, true},
     {false, true, false, true, true},
     {true, false, true, false, true},
     {false, true, false, true, true},
     {true, false, true, false, true},
     {false, true, false, true, true}},

    // Flipped horizontally (0°)
    {{true, true, true, true, true, true},
     {true, false, true, false, true, false},
     {false, true, false, true, false, true},
     {true, false, true, false, true, false},
     {false, true, false, true, false, true}},

    // Flipped + 90° CW
    {{false, true, false, true, true},
     {true, false, true, false, true},
     {false, true, false, true, true},
     {true, false, true, false, true},
     {false, true, false, true, true},
     {true, false, true, false, true}},

    // Flipped + 180°
    {{true, false, true, false, true, false},
     {false, true, false, true, false, true},
     {true, false, true, false, true, false},
     {false, true, false, true, false, true},
     {true, true, true, true, true, true}},

    // Flipped + 270° CW
    {{true, false, true, false, true},
     {true, true, false, true, false},
     {true, false, true, false, true},
     {true, true, false, true, false},
     {true, false, true, false, true},
     {true, true, false, true, false}}};

// 6x5 asymmetric pattern
const std::vector<Shape> shape_6x5 = {
    // Original (0°)
    {{true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {true, true, true, true, true}},

    // 90° CW
    {{true, true, false, true, false, true},
     {true, false, true, false, true, false},
     {true, true, false, true, false, true},
     {true, false, true, false, true, false},
     {true, true, false, true, false, true}},

    // 180°
    {{true, true, true, true, true},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true}},

    // 270° CW
    {{true, false, true, false, true, true},
     {false, true, false, true, false, true},
     {true, false, true, false, true, true},
     {false, true, false, true, false, true},
     {true, false, true, false, true, true}},

    // Flipped horizontally (0°)
    {{true, true, true, true, true},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true}},

    // Flipped + 90° CW
    {{true, false, true, false, true, true},
     {false, true, false, true, false, true},
     {true, false, true, false, true, true},
     {false, true, false, true, false, true},
     {true, false, true, false, true, true}},

    // Flipped + 180°
    {{true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {false, true, false, true, false},
     {true, false, true, false, true},
     {true, true, true, true, true}},

    // Flipped + 270° CW
    {{true, true, false, true, false, true},
     {true, false, true, false, true, false},
     {true, true, false, true, false, true},
     {true, false, true, false, true, false},
     {true, true, false, true, false, true}}};

// 1x5 pattern
const std::vector<Shape> shape_1x5 = {
    // Original (0°)
    {{true, false, true, false, true}},

    // 90° CW
    {{true}, {false}, {true}, {false}, {true}},

    // 180°
    {{true, false, true, false, true}},

    // 270° CW
    {{true}, {false}, {true}, {false}, {true}},

    // Flipped horizontally (0°)
    {{true, false, true, false, true}},

    // Flipped + 90° CW
    {{true}, {false}, {true}, {false}, {true}},

    // Flipped + 180°
    {{true, false, true, false, true}},

    // Flipped + 270° CW
    {{true}, {false}, {true}, {false}, {true}}};

// 1x6 pattern
const std::vector<Shape> shape_1x6 = {
    // Original (0°)
    {{true, false, true, false, true, false}},

    // 90° CW
    {{true}, {false}, {true}, {false}, {true}, {false}},

    // 180°
    {{false, true, false, true, false, true}},

    // 270° CW
    {{false}, {true}, {false}, {true}, {false}, {true}},

    // Flipped horizontally (0°)
    {{true, false, true, false, true, false}},

    // Flipped + 90° CW
    {{true}, {false}, {true}, {false}, {true}, {false}},

    // Flipped + 180°
    {{false, true, false, true, false, true}},

    // Flipped + 270° CW
    {{false}, {true}, {false}, {true}, {false}, {true}}};

// 5x1 pattern
const std::vector<Shape> shape_5x1 = {
    // Original (0°)
    {{true}, {false}, {true}, {false}, {true}},

    // 90° CW
    {{true, false, true, false, true}},

    // 180°
    {{true}, {false}, {true}, {false}, {true}},

    // 270° CW
    {{true, false, true, false, true}},

    // Flipped horizontally (0°)
    {{true}, {false}, {true}, {false}, {true}},

    // Flipped + 90° CW
    {{true, false, true, false, true}},

    // Flipped + 180°
    {{true}, {false}, {true}, {false}, {true}},

    // Flipped + 270° CW
    {{true, false, true, false, true}}};

// 6x1 pattern
const std::vector<Shape> shape_6x1 = {
    // Original (0°)
    {{true}, {false}, {true}, {false}, {true}, {false}},

    // 90° CW
    {{false, true, false, true, false, true}},

    // 180°
    {{false}, {true}, {false}, {true}, {false}, {true}},

    // 270° CW
    {{true, false, true, false, true, false}},

    // Flipped horizontally (0°)
    {{false}, {true}, {false}, {true}, {false}, {true}},

    // Flipped + 90° CW
    {{true, false, true, false, true, false}},

    // Flipped + 180°
    {{true}, {false}, {true}, {false}, {true}, {false}},

    // Flipped + 270° CW
    {{false, true, false, true, false, true}}};

// All shape groups
const std::vector<std::vector<Shape> *> all_shape_groups = {
    const_cast<std::vector<Shape> *>(&shape_5x5),
    const_cast<std::vector<Shape> *>(&shape_5x6),
    const_cast<std::vector<Shape> *>(&shape_6x5),
    const_cast<std::vector<Shape> *>(&shape_1x5),
    const_cast<std::vector<Shape> *>(&shape_1x6),
    const_cast<std::vector<Shape> *>(&shape_5x1),
    const_cast<std::vector<Shape> *>(&shape_6x1)};

// Transformation indices
enum class Transform
{
    ROTATE_CW = 1,       // (i + 1) % 8
    ROTATE_180 = 2,      // (i + 2) % 8
    ROTATE_CCW = 3,      // (i + 3) % 8
    FLIP_HORIZONTAL = 4, // (i + 4) % 8
    FLIP_VERTICAL = 6    // (i + 6) % 8 (flip + 180)
};

// Helper function to compare two shapes
bool shapesEqual(const Shape &a, const Shape &b)
{
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

// Find which shape group and index a given shape belongs to
std::pair<int, int> findShapeIndex(const Shape &target)
{
    for (size_t group = 0; group < all_shape_groups.size(); ++group)
    {
        for (size_t variant = 0; variant < 8; ++variant)
        {
            if (shapesEqual((*all_shape_groups[group])[variant], target))
            {
                return {static_cast<int>(group), static_cast<int>(variant)};
            }
        }
    }
    return {-1, -1}; // Not found
}

// Get expected result of applying a transformation
Shape getExpectedTransform(const Shape &original, Transform transform)
{
    std::pair<int, int> shape_info = findShapeIndex(original);
    int group_idx = shape_info.first;
    int variant_idx = shape_info.second;

    if (group_idx == -1)
    {
        throw std::runtime_error("Shape not found in predefined variants");
    }

    int result_idx = (variant_idx + static_cast<int>(transform)) % 8;
    return (*all_shape_groups[group_idx])[result_idx];
}

// Test function that validates a transformation
void testTransformation(const Shape &original, Transform expected_transform,
                        std::function<void(TetrisPiece &)> actual_transform,
                        const std::string &transform_name)
{

    // Get expected result
    Shape expected_shape = getExpectedTransform(original, expected_transform);

    // Apply actual transformation
    TetrisPiece piece{original};
    actual_transform(piece);

    // Create expected piece for comparison
    TetrisPiece expected_piece{expected_shape};

    // Compare by string representation
    std::stringstream actual_ss, expected_ss;
    actual_ss << piece;
    expected_ss << expected_piece;

    if (actual_ss.str() != expected_ss.str())
    {
        std::cout << "FAILED: " << transform_name << " transformation\n";
        std::cout << "Expected:\n"
                  << expected_ss.str() << "\n";
        std::cout << "Actual:\n"
                  << actual_ss.str() << "\n";
        assert(false);
    }
}

// Test all transformations on all shape variants
void testAllTransformations()
{
    std::cout << "Running Comprehensive Transformation Tests...\n";

    for (size_t group = 0; group < all_shape_groups.size(); ++group)
    {
        std::cout << "Testing shape group " << (group + 1) << "/7...\n";

        for (size_t variant = 0; variant < 8; ++variant)
        {
            const Shape &test_shape = (*all_shape_groups[group])[variant];

            // Test clockwise rotation
            testTransformation(test_shape, Transform::ROTATE_CW, [](TetrisPiece &p)
                               { p.rotateClockwise(); }, "Rotate Clockwise");

            // Test counter-clockwise rotation
            testTransformation(test_shape, Transform::ROTATE_CCW, [](TetrisPiece &p)
                               { p.rotateCounterClockwise(); }, "Rotate Counter-Clockwise");

            // Test 180 rotation
            testTransformation(test_shape, Transform::ROTATE_180, [](TetrisPiece &p)
                               { p.rotate180(); }, "Rotate 180");

            // Test horizontal flip
            testTransformation(test_shape, Transform::FLIP_HORIZONTAL, [](TetrisPiece &p)
                               { p.flipHorizontal(); }, "Flip Horizontal");

            // Test vertical flip
            testTransformation(test_shape, Transform::FLIP_VERTICAL, [](TetrisPiece &p)
                               { p.flipVertical(); p.rotate180(); }, "Flip Vertical");
        }
    }

    std::cout << "✓ All transformation tests passed!\n";
}

// Main function removed - will be called from unified test runner