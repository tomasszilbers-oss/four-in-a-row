#include "Rules.h"

CheckResult Rules::CheckWinTie(const Board& board)
{
    // TODO: implement 4-in-a-row checks (horizontal/vertical/diagonals)

    if (board.IsFull())
        return {Outcome::Tie, std::nullopt};

    return {Outcome::Playing, std::nullopt};
}
