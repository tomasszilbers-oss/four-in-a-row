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

    // In vs-computer mode only the human clicks.
    if (vsComputer_ && currentPlayer_ != Player::Human) return;

    ApplyMove(col, currentPlayer_);
}

void GameController::ApplyMove(int col, Player player)
{
    auto placedRow = board_.DropToken(col, player);
    if (!placedRow.has_value())
        return;

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

    // Optional: if you already have an AI move function, trigger it here.
    // This block can stay disabled until AI is implemented.
    /*
    if (vsComputer_ && currentPlayer_ == Player::Computer && state_ == GameState::Playing)
    {
        int col = ai_.ChooseMove(board_); // Must return 0..6
        ApplyMove(col, Player::Computer);
    }
    */
}

void GameController::ApplyMoveFromUI(int col)
{
    if (state_ != GameState::Playing)
        return;

    ApplyMove(col, currentPlayer_);
}
