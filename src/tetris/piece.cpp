#include "tetris/piece.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <functional>

TetrisPiece::TetrisPiece(std::vector<std::vector<bool>> shape)
{
    // Height must be greater than zero
    if ((width = shape.size()) == 0)
    {
        throw std::invalid_argument("Width of piece must be greater than zero");
    }

    // All columns must have the same height
    height = shape.at(0).size();
    for (auto row : shape)
    {
        if (row.size() != height)
        {
            throw std::invalid_argument("Tetris piece must have uniform height");
        }
    }

    // Height must be greater than zero
    if (height == 0)
    {
        throw std::invalid_argument("Tetris piece height must be greater than zero");
    }

    // Initialize grid
    grid = new std::vector<std::vector<bool> *>(width);
    for (size_t col_idx = 0; col_idx < width; col_idx++)
    {
        (*grid)[col_idx] = new std::vector<bool>(shape.at(col_idx));
    }
}

TetrisPiece::~TetrisPiece() {
    for (size_t i = 0; i < width; i++) {
        delete grid->at(i);
    }
    delete grid;
}

void TetrisPiece::flipHorizontal()
{
    for (size_t col_idx = 0; col_idx < width / 2; col_idx++)
    {
        std::vector<bool> *temp_col = grid->at(col_idx);
        grid->at(col_idx) = grid->at(width - col_idx - 1);
        grid->at(width - col_idx - 1) = temp_col;
    }
}

void TetrisPiece::flipVertical()
{
    for (size_t col_idx = 0; col_idx < width; col_idx++)
    {
        std::vector<bool>& col = *(grid->at(col_idx));
        for (size_t row_idx = 0; row_idx < height / 2; row_idx++)
        {
            bool temp_val = col.at(row_idx);
            col.at(row_idx) = col.at(height - row_idx - 1);
            col.at(height - row_idx - 1) = temp_val;
        }
    }
}

void TetrisPiece::rotateCounterClockwise()
{
    // Create new grid
    std::vector<std::vector<bool> *> *new_grid = new std::vector<std::vector<bool> *>(height);

    // Fill out the new grid
    for (size_t new_col_idx = 0; new_col_idx < height; new_col_idx++)
    {
        new_grid->at(new_col_idx) = new std::vector<bool>(width);
        for (size_t new_row_idx = 0; new_row_idx < width; new_row_idx++)
        {
            new_grid->at(new_col_idx)->at(new_row_idx) = grid->at(new_row_idx)->at(height - 1 - new_col_idx);
        }
    }

    // Delete the old grid
    for (size_t col_idx = 0; col_idx < width; col_idx++)
    {
        delete grid->at(col_idx);
    }
    delete grid;
    grid = nullptr;

    // Swap width and height
    size_t temp_val = width;
    width = height;
    height = temp_val;

    // Save the new grid
    grid = new_grid;
}

void TetrisPiece::rotateClockwise()
{
    // Create new grid
    std::vector<std::vector<bool> *> *new_grid = new std::vector<std::vector<bool> *>(height);

    // Fill out the new grid
    for (size_t new_col_idx = 0; new_col_idx < height; new_col_idx++)
    {
        new_grid->at(new_col_idx) = new std::vector<bool>(width);
        for (size_t new_row_idx = 0; new_row_idx < width; new_row_idx++)
        {
            new_grid->at(new_col_idx)->at(new_row_idx) = grid->at(width - 1 - new_row_idx)->at(new_col_idx);
        }
    }

    // Delete the old grid
    for (size_t col_idx = 0; col_idx < width; col_idx++)
    {
        delete grid->at(col_idx);
    }
    delete grid;
    grid = nullptr;

    // Swap width and height
    size_t temp_val = width;
    width = height;
    height = temp_val;

    // Save the new grid
    grid = new_grid;
}

void TetrisPiece::rotate180()
{
    // Swap opposite values
    size_t required_swaps = (width * height) / 2;
    size_t swaps = 0;
    for (size_t new_col_idx = 0; new_col_idx < width; new_col_idx++)
    {
        for (size_t new_row_idx = 0; new_row_idx < height; new_row_idx++)
        {
            bool temp_val = grid->at(new_col_idx)->at(new_row_idx);
            grid->at(new_col_idx)->at(new_row_idx) = grid->at(width - 1 - new_col_idx)->at(height - 1 - new_row_idx);
            grid->at(width - 1 - new_col_idx)->at(height - 1 - new_row_idx) = temp_val;

            // Early return to avoid un-doing swaps
            ++swaps;
            if (swaps >= required_swaps)
            {
                return;
            }
        }
    }
}

size_t TetrisPiece::lowestBlockInColumn(size_t col_idx)
{
    const std::vector<bool>& column = *(grid->at(col_idx));
    for (size_t row_idx = 0; row_idx < height; row_idx++)
    {
        if (column.at(row_idx))
        {
            return row_idx;
        }
    }

    // No blocks present in current column, return largest possible value
    return std::numeric_limits<size_t>::max();
};

std::ostream &operator<<(std::ostream &outs, const TetrisPiece &piece)
{
    std::string horizontal_bar(piece.width + 2, '-');
    horizontal_bar += '\n';
    outs << horizontal_bar;
    for (size_t row_idx = piece.height; row_idx-- > 0;)
    {
        outs << '|';
        for (size_t col_idx = 0; col_idx < piece.width; col_idx++)
        {
            if (piece.grid->at(col_idx)->at(row_idx))
            {
                outs << 'X';
            }
            else
            {
                outs << ' ';
            }
        }
        outs << "|\n";
    }
    outs << horizontal_bar;
    return outs;
}

TetrisPiece TetrisPiece::createQPiece()
{
    return TetrisPiece{{{true, true}, {true, true}}};
}

TetrisPiece TetrisPiece::createZPiece()
{
    return TetrisPiece{{{false, true}, {true, true}, {true, false}}};
}

TetrisPiece TetrisPiece::createTPiece()
{
    return TetrisPiece{{{false, true}, {true, true}, {false, true}}};
}

TetrisPiece TetrisPiece::createIPiece()
{
    return TetrisPiece{{{true}, {true}, {true}, {true}}};
}

TetrisPiece TetrisPiece::createLPiece()
{
    return TetrisPiece{{{true, true, true}, {true, false, false}}};
}

const std::map<char, std::function<TetrisPiece()>> TetrisPiece::pieceFactories = {
    {'Q', TetrisPiece::createQPiece},
    {'Z', TetrisPiece::createZPiece},
    {'T', TetrisPiece::createTPiece},
    {'I', TetrisPiece::createIPiece},
    {'L', TetrisPiece::createLPiece},
};