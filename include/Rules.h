#pragma once
#include "Board.h"
#include <optional>

enum class Outcome
{
    Playing,
    Win,
    Tie
};

struct CheckResult
{
    Outcome outcome = Outcome::Playing;
    std::optional<Player> winner; // set only when outcome == Win
};

class Rules
{
public:
    static CheckResult CheckWinTie(const Board& board);
};
