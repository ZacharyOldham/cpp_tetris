#include <iostream>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <limits>
#include <string>

#include "tetris/piece.h"
#include "test_pieces.cpp"
#include <gtest/gtest.h>

TEST(BasicPiece, ConstructorValid)
{
    EXPECT_NO_THROW(TetrisPiece piece({{true, true}, {false, false}}));
}

TEST(BasicPiece, ConstructorZeroWidth)
{
    EXPECT_THROW(TetrisPiece piece{{{}}}, std::invalid_argument);
}

TEST(BasicPiece, ConstructorZeroHeight)
{
    EXPECT_THROW(TetrisPiece piece({{}, {}}), std::invalid_argument);
}

TEST(BasicPiece, ConstructorNonUniformHeight)
{
    EXPECT_THROW(TetrisPiece piece({{true}, {true, false}}), std::invalid_argument);
}

TEST(BasicPiece, FactoryFunctions)
{
    EXPECT_NO_THROW({
        TetrisPiece q_piece = TetrisPiece::createQPiece();
        TetrisPiece z_piece = TetrisPiece::createZPiece();
        TetrisPiece t_piece = TetrisPiece::createTPiece();
        TetrisPiece i_piece = TetrisPiece::createIPiece();
        TetrisPiece l_piece = TetrisPiece::createLPiece();
    });
}

TEST(BasicPiece, FactoryFunctionMap)
{
    TetrisPiece piece1 = TetrisPiece::createQPiece();
    TetrisPiece piece2 = TetrisPiece::pieceFactories.at('Q')();
    EXPECT_EQ(piece1, piece2);
}

TEST(BasicPiece, EdgeCaseOneColumnOneRow)
{
    TetrisPiece piece{{{true}}};
    TetrisPiece expected{{{true}}};

    // All transformations should leave single block unchanged
    EXPECT_EQ(piece, expected);
    piece.flipHorizontal();
    EXPECT_EQ(piece, expected);
    piece.flipVertical();
    EXPECT_EQ(piece, expected);
    piece.rotateClockwise();
    EXPECT_EQ(piece, expected);
    piece.rotateCounterClockwise();
    EXPECT_EQ(piece, expected);
    piece.rotate180();
    EXPECT_EQ(piece, expected);
}

TEST(BasicPiece, LowestBlockNormal)
{
    TetrisPiece piece{{{false, true, false}, {true, false, true}}};

    EXPECT_EQ(piece.lowestBlockInColumn(0), 1);
    EXPECT_EQ(piece.lowestBlockInColumn(1), 0);
}

TEST(BasicPiece, LowestBlockEmptyColumn)
{
    TetrisPiece piece{{{true, false}, {false, false}}};

    EXPECT_EQ(piece.lowestBlockInColumn(1), std::numeric_limits<size_t>::max());
}

TEST(BasicPiece, StreamOperator)
{
    TetrisPiece piece{{{true}}};
    std::stringstream ss;
    ss << piece;

    std::string output = ss.str();
    // Should contain borders and the X
    EXPECT_EQ(output, "---\n|X|\n---\n");
}

struct PieceTransformationTestData
{
    std::vector<Shape> shapes;
    std::string shape_name;
    int expected_idx;
    std::function<void(TetrisPiece &)> transformation;
    std::string transformation_name;
};

class PieceTransformationTest : public ::testing::TestWithParam<PieceTransformationTestData>
{
};

TEST_P(PieceTransformationTest, TransformPiece)
{
    const auto &param = GetParam();
    auto shapes = param.shapes;
    auto shape_name = param.shape_name;
    auto expected_idx = param.expected_idx;
    auto transformation = param.transformation;
    auto transformation_name = param.transformation_name;

    Shape &base_shape = shapes[0];
    Shape &expected_shape = shapes[expected_idx];

    TetrisPiece computed{base_shape};
    transformation(computed);
    TetrisPiece hardcoded{expected_shape};

    std::stringstream failure_stream = std::stringstream();
    failure_stream << "ERROR: " << transformation_name << " mismatch in " << shape_name << "\n";
    failure_stream << "Hardcoded:\n";
    failure_stream << hardcoded << "\n";
    failure_stream << "Computed:\n";
    failure_stream << computed << "\n";
    EXPECT_EQ(computed, hardcoded);
}

std::vector<PieceTransformationTestData> GetAdvancedPieceTransformationTestParams()
{
    std::vector<std::tuple<std::vector<Shape>, std::string>> shapes_tuples = {
        std::make_tuple(shape_5x5, "5x5"),
        std::make_tuple(shape_5x6, "5x6"),
        std::make_tuple(shape_6x5, "6x5"),
        std::make_tuple(shape_1x5, "1x5"),
        std::make_tuple(shape_1x6, "1x6"),
        std::make_tuple(shape_5x1, "5x1"),
        std::make_tuple(shape_6x1, "6x1"),
    };

    std::vector<std::tuple<int, std::function<void(TetrisPiece &)>, std::string>> transformation_tuples = {
        std::make_tuple(1, [](TetrisPiece &p)
                        { p.rotateClockwise(); }, "90° CW rotation"),
        std::make_tuple(2, [](TetrisPiece &p)
                        { p.rotate180(); }, "180° rotation"),
        std::make_tuple(3, [](TetrisPiece &p)
                        { p.rotateCounterClockwise(); }, "270° CW rotation"),
        std::make_tuple(4, [](TetrisPiece &p)
                        { p.flipHorizontal(); }, "Horizontal flip"),
        std::make_tuple(6, [](TetrisPiece &p)
                        { p.flipVertical(); }, "Vertical flip"),
        std::make_tuple(5, [](TetrisPiece &p)
                        { p.flipHorizontal(); p.rotateClockwise(); }, "Flipped + 90° CW"),
        std::make_tuple(7, [](TetrisPiece &p)
                        { p.flipHorizontal(); p.rotateCounterClockwise(); }, "Flipped + 270° CW"),
    };

    std::vector<PieceTransformationTestData> parameters = std::vector<PieceTransformationTestData>();

    for (auto shapes_tuple : shapes_tuples)
    {
        for (auto transformation_tuple : transformation_tuples)
        {
            parameters.push_back(PieceTransformationTestData{
                std::get<std::vector<Shape>>(shapes_tuple),
                std::get<std::string>(shapes_tuple),
                std::get<int>(transformation_tuple),
                std::get<std::function<void(TetrisPiece &)>>(transformation_tuple),
                std::get<std::string>(transformation_tuple),
            });
        }
    }
    return parameters;
}

INSTANTIATE_TEST_SUITE_P(
    AdvancedPieceTransformation,
    PieceTransformationTest,
    ::testing::ValuesIn(GetAdvancedPieceTransformationTestParams()));
