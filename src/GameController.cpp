#include "GameController.h"

GameController::GameController() = default;

void GameController::NewGame(bool vsComputer)
{
    vsComputer_ = vsComputer;
    board_.Reset();

    state_ = GameState::Playing;
    currentPlayer_ = Player::Human;

    outcome_ = Outcome::Playing;
    winner_.reset();
}

void GameController::OnHumanChooseColumn(int col)
{
    if (state_ != GameState::Playing) return;
    if (currentPlayer_ != Player::Human) return;

    ApplyMove(col, Player::Human);
}

void GameController::ApplyMove(int col, Player player)
{
    auto placedRow = board_.DropToken(col, player);
    if (!placedRow.has_value())
        return; // column full or invalid -> ignore / UI can show message

    auto res = Rules::CheckWinTie(board_);
    outcome_ = res.outcome;
    winner_ = res.winner;

    if (outcome_ == Outcome::Win || outcome_ == Outcome::Tie)
    {
        state_ = GameState::GameOver;
        return;
    }

    NextTurn();
}

void GameController::NextTurn()
{
    currentPlayer_ = (currentPlayer_ == Player::Human) ? Player::Computer : Player::Human;

    // TODO later: if vsComputer_ and currentPlayer_ == Computer -> call AI, then ApplyMove(aiCol, Computer)
}
