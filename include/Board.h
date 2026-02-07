#pragma once
#include "Player.h"
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
    std::optional<int> DropToken(int col, Player player);

    // Returns who occupies cell, or nullopt if empty.
    std::optional<Player> GetCell(int col, int row) const;

    bool IsFull() const;

private:
    // grid[col][row]
    std::array<std::array<std::optional<Player>, Rows>, Cols> grid_;
};
