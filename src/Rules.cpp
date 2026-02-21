#include "Rules.h"

namespace
{
    bool CheckDirection(const Board& board, int startCol, int startRow,
                        int dCol, int dRow, Player player)
    {
        for (int i = 1; i < 4; ++i)
        {
            int c = startCol + dCol * i;
            int r = startRow + dRow * i;

            auto cell = board.GetCell(c, r);
            if (!cell.has_value() || *cell != player)
                return false;
        }
        return true;
    }
}

CheckResult Rules::CheckWinTie(const Board& board)
{
    for (int col = 0; col < Board::Cols; ++col)
    {
        for (int row = 0; row < Board::Rows; ++row)
        {
            auto cell = board.GetCell(col, row);
            if (!cell.has_value())
                continue;

            Player player = *cell;

            // Horizontal →
            if (CheckDirection(board, col, row, 1, 0, player))
                return {Outcome::Win, player};

            // Vertical ↑
            if (CheckDirection(board, col, row, 0, 1, player))
                return {Outcome::Win, player};

            // Diagonal ↗
            if (CheckDirection(board, col, row, 1, 1, player))
                return {Outcome::Win, player};

            // Diagonal ↖
            if (CheckDirection(board, col, row, -1, 1, player))
                return {Outcome::Win, player};
        }
    }

    if (board.IsFull())
        return {Outcome::Tie, std::nullopt};

    return {Outcome::Playing, std::nullopt};
}
