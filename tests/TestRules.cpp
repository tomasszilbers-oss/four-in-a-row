#include <iostream>
#include <cassert>
#include "Board.h"
#include "Rules.h"

void TestHorizontalWin()
{
    Board b;

    b.DropToken(0, 0);
    b.DropToken(1, 0);
    b.DropToken(2, 0);
    b.DropToken(3, 0);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Win);
    assert(res.winnerIndex.has_value());
    assert(*res.winnerIndex == 0);

    std::cout << "Horizontal win test passed\n";
}

void TestVerticalWin()
{
    Board b;

    for (int i = 0; i < 4; ++i)
        b.DropToken(0, 0);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Win);
    assert(res.winnerIndex.has_value());
    assert(*res.winnerIndex == 0);

    std::cout << "Vertical win test passed\n";
}

void TestDiagonalUpRight()
{
    Board b;

    b.DropToken(0, 0);

    b.DropToken(1, 1);
    b.DropToken(1, 0);

    b.DropToken(2, 1);
    b.DropToken(2, 1);
    b.DropToken(2, 0);

    b.DropToken(3, 1);
    b.DropToken(3, 1);
    b.DropToken(3, 1);
    b.DropToken(3, 0);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Win);
    assert(res.winnerIndex.has_value());
    assert(*res.winnerIndex == 0);

    std::cout << "Diagonal ↗ test passed\n";
}

void TestDiagonalUpLeft()
{
    Board b;

    b.DropToken(3, 0);

    b.DropToken(2, 1);
    b.DropToken(2, 0);

    b.DropToken(1, 1);
    b.DropToken(1, 1);
    b.DropToken(1, 0);

    b.DropToken(0, 1);
    b.DropToken(0, 1);
    b.DropToken(0, 1);
    b.DropToken(0, 0);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Win);
    assert(res.winnerIndex.has_value());
    assert(*res.winnerIndex == 0);

    std::cout << "Diagonal ↖ test passed\n";
}

void TestTie()
{
    Board b;

    int pattern[Board::Cols][Board::Rows] =
    {
        {0,0,1,1,0,0},
        {1,1,0,0,1,1},
        {0,0,1,1,0,0},
        {1,1,0,0,1,1},
        {0,0,1,1,0,0},
        {1,1,0,0,1,1},
        {0,0,1,1,0,0}
    };

    for (int col = 0; col < Board::Cols; ++col)
        for (int row = 0; row < Board::Rows; ++row)
            b.DropToken(col, pattern[col][row]);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Tie);
    assert(!res.winnerIndex.has_value());

    std::cout << "Tie test passed\n";
}

int main()
{
    TestHorizontalWin();
    TestVerticalWin();
    TestDiagonalUpRight();
    TestDiagonalUpLeft();
    TestTie();

    std::cout << "\nAll tests passed successfully.\n";
    return 0;
}