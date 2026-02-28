#pragma once
#include <array>
#include <optional>

class Board
{
public:
    static constexpr int Cols = 7;
    static constexpr int Rows = 6;

    Board();

    void Reset();

    bool CanDrop(int col) const;

    // Returns placed row index (0..Rows-1) if success, nullopt if column is full/invalid.
    std::optional<int> DropToken(int col, int playerIndex);

    // Returns who occupies cell, or nullopt if empty.
    std::optional<int> GetCell(int col, int row) const;

    // Returns the row index (0..Rows-1) where a token would land if dropped,
    // or nullopt if the column is full/invalid. Does not modify the board.
    std::optional<int> GetDropRow(int col) const;

    bool IsFull() const;

private:
    // grid[col][row]
    std::array<std::array<std::optional<int>, Rows>, Cols> grid_;
};
