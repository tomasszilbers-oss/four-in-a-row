#include <iostream>
#include <cassert>
#include "Board.h"
#include "Rules.h"

void TestHorizontalWin()
{
    Board b;

    b.DropToken(0, Player::Human);
    b.DropToken(1, Player::Human);
    b.DropToken(2, Player::Human);
    b.DropToken(3, Player::Human);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Win);
    assert(res.winner.has_value());
    assert(*res.winner == Player::Human);

    std::cout << "Horizontal win test passed\n";
}

void TestVerticalWin()
{
    Board b;

    for (int i = 0; i < 4; ++i)
        b.DropToken(0, Player::Human);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Win);
    assert(*res.winner == Player::Human);

    std::cout << "Vertical win test passed\n";
}

void TestDiagonalUpRight()
{
    Board b;

    // Construct ↗ diagonal
    b.DropToken(0, Player::Human);

    b.DropToken(1, Player::Computer);
    b.DropToken(1, Player::Human);

    b.DropToken(2, Player::Computer);
    b.DropToken(2, Player::Computer);
    b.DropToken(2, Player::Human);

    b.DropToken(3, Player::Computer);
    b.DropToken(3, Player::Computer);
    b.DropToken(3, Player::Computer);
    b.DropToken(3, Player::Human);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Win);
    assert(*res.winner == Player::Human);

    std::cout << "Diagonal ↗ test passed\n";
}

void TestDiagonalUpLeft()
{
    Board b;

    // Construct ↖ diagonal
    b.DropToken(3, Player::Human);

    b.DropToken(2, Player::Computer);
    b.DropToken(2, Player::Human);

    b.DropToken(1, Player::Computer);
    b.DropToken(1, Player::Computer);
    b.DropToken(1, Player::Human);

    b.DropToken(0, Player::Computer);
    b.DropToken(0, Player::Computer);
    b.DropToken(0, Player::Computer);
    b.DropToken(0, Player::Human);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Win);
    assert(*res.winner == Player::Human);

    std::cout << "Diagonal ↖ test passed\n";
}

void TestTie()
{
    Board b;

    // Carefully constructed pattern that avoids 4 in a row
    Player pattern[Board::Cols][Board::Rows] =
    {
        {Player::Human, Player::Human, Player::Computer, Player::Computer, Player::Human, Player::Human},
        {Player::Computer, Player::Computer, Player::Human, Player::Human, Player::Computer, Player::Computer},
        {Player::Human, Player::Human, Player::Computer, Player::Computer, Player::Human, Player::Human},
        {Player::Computer, Player::Computer, Player::Human, Player::Human, Player::Computer, Player::Computer},
        {Player::Human, Player::Human, Player::Computer, Player::Computer, Player::Human, Player::Human},
        {Player::Computer, Player::Computer, Player::Human, Player::Human, Player::Computer, Player::Computer},
        {Player::Human, Player::Human, Player::Computer, Player::Computer, Player::Human, Player::Human}
    };

    for (int col = 0; col < Board::Cols; ++col)
        for (int row = 0; row < Board::Rows; ++row)
            b.DropToken(col, pattern[col][row]);

    auto res = Rules::CheckWinTie(b);

    assert(res.outcome == Outcome::Tie);

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
