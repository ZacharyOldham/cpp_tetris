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
