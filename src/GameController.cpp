#include "GameController.h"

GameController::GameController() = default;

void GameController::NewGame(bool vsComputer,
                             const std::string& name1,
                             const std::string& name2,
                             bool firstPlayerStarts)
{
    vsComputer_ = vsComputer;

    board_.Reset();
    state_ = GameState::Playing;

    players_[0] = {name1, Player::Human};
    players_[1] = {name2, vsComputer ? Player::Computer : Player::Human};

    firstPlayerStarts_ = firstPlayerStarts;
    currentPlayerIndex_ = firstPlayerStarts ? 0 : 1;

    outcome_ = Outcome::Playing;
    winnerIndex_.reset();
}

void GameController::OnHumanChooseColumn(int col)
{
    if (state_ != GameState::Playing)
        return;

    const PlayerInfo& current = players_[currentPlayerIndex_];

    // In vs-computer mode only human can click
    if (vsComputer_ && current.type != Player::Human)
        return;

    ApplyMove(col, currentPlayerIndex_);
}

void GameController::ApplyMove(int col, int playerIndex)
{
    auto placedRow = board_.DropToken(col, playerIndex);
    if (!placedRow.has_value())
        return;

    auto res = Rules::CheckWinTie(board_);
    outcome_ = res.outcome;
    winnerIndex_ = res.winnerIndex;

    if (outcome_ == Outcome::Win || outcome_ == Outcome::Tie)
    {
        state_ = GameState::GameOver;
        return;
    }

    NextTurn();
}

void GameController::NextTurn()
{
    currentPlayerIndex_ = 1 - currentPlayerIndex_;
}

void GameController::ApplyMoveFromUI(int col)
{
    if (state_ != GameState::Playing)
        return;

    ApplyMove(col, currentPlayerIndex_);
}