#ifndef TETRIS_PIECE_H
#define TETRIS_PIECE_H

#include <vector>
#include <iostream>
#include <map>
#include <functional>

class TetrisPiece
{
private:
    size_t width;
    size_t height;

public:
    std::vector<std::vector<bool> *> *grid;
    /**
     * @brief Construct a new Tetris Piece from a 2D shape vector
     *
     * @param shape A column-major 2D vector of bools, where true indicates that part of the piece is present at that part of the grid.
     *
     * @throws std:invalid_argument if width or height is zero, or if height is non-uniform
     */
    TetrisPiece(std::vector<std::vector<bool>> shape);
    ~TetrisPiece();

    /**
     * @brief Flip the piece horizontally (reorder columns)
     *
     */
    void flipHorizontal();

    /**
     * @brief Flip the piece vertically (reorder rows)
     *
     */
    void flipVertical();

    /**
     * @brief Rotate the piece 90 degrees clockwise
     *
     */
    void rotateClockwise();

    /**
     * @brief Rotate the piece 90 degrees counter clockwise
     *
     */
    void rotateCounterClockwise();

    /**
     * @brief Rotate the piece 180 degrees
     *
     */
    void rotate180();

    /**
     * @brief Determine the height of the lowest block in the given column
     *
     * @param col_idx The index of the column to find the lowest block in
     * @return The height of the lowest block in the given column (zero indexed). If no blocks are present in the given column the max size_t is returned.
     */
    size_t lowestBlockInColumn(size_t col_idx);

    /**
     * @brief Output a string representation of a tetris piece to the given output stream
     *
     * @param outs reference to the output stream
     * @param piece reference to the tetris piece
     * @return std::ostream& the provided output stream
     */
    friend std::ostream &operator<<(std::ostream &outs, const TetrisPiece &piece);

    /**
     * @brief Create and return a "Q" piece (2x2 square) which looks like:
     *
     * XX
     *
     * XX
     *
     * @return A "Q" piece
     */
    static TetrisPiece createQPiece();

    /**
     * @brief Create and return a "Z" piece which looks like:
     *
     * XX_
     *
     * _XX
     *
     * @return A "Z" piece
     */
    static TetrisPiece createZPiece();

    /**
     * @brief Create and return a "T" piece which looks like:
     *
     * XXX
     *
     * _X_
     *
     * @return A "T" piece
     */
    static TetrisPiece createTPiece();

    /**
     * @brief Create and return an "I" piece which looks like:
     *
     * XXXX
     *
     * @return An "I" piece
     */
    static TetrisPiece createIPiece();

    /**
     * @brief Create and return an "L" piece which looks like:
     *
     * X_
     *
     * X_
     *
     * XX
     *
     * @return An "L" piece
     */
    static TetrisPiece createLPiece();

    /**
     * @brief Map from piece names to factory functions for those pieces
     *
     */
    const static std::map<char, std::function<TetrisPiece()>> pieceFactories;
};

#endif // TETRIS_PIECE_H