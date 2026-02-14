#include "Board.h"

Board::Board()
{
    Reset();
}

void Board::Reset()
{
    for (auto& col : grid_)
        for (auto& cell : col)
            cell.reset();
}

bool Board::CanDrop(int col) const
{
    if (col < 0 || col >= Cols) return false;
    return !grid_[col][Rows - 1].has_value();
}

std::optional<int> Board::DropToken(int col, Player player)
{
    if (!CanDrop(col)) return std::nullopt;

    for (int row = 0; row < Rows; ++row)
    {
        if (!grid_[col][row].has_value())
        {
            grid_[col][row] = player;
            return row;
        }
    }
    return std::nullopt;
}

std::optional<Player> Board::GetCell(int col, int row) const
{
    if (col < 0 || col >= Cols || row < 0 || row >= Rows) return std::nullopt;
    return grid_[col][row];
}

std::optional<int> Board::GetDropRow(int col) const
{
    if (col < 0 || col >= Cols)
        return std::nullopt;

    for (int row = 0; row < Rows; ++row)
    {
        if (!grid_[col][row].has_value())
            return row;
    }
    return std::nullopt;
}

bool Board::IsFull() const
{
    for (int col = 0; col < Cols; ++col)
        if (CanDrop(col)) return false;
    return true;
}
