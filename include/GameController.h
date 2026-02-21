#pragma once
#include "Board.h"
#include "Rules.h"
#include "GameState.h"

class GameController
{
public:
    GameController();

    void NewGame(bool vsComputer);

    // Called by UI when user selected a column.
    void OnHumanChooseColumn(int col);

    void ApplyMoveFromUI(int col);

    const Board& GetBoard() const { return board_; }
    GameState GetState() const { return state_; }

    Outcome GetOutcome() const { return outcome_; }
    std::optional<Player> GetWinner() const { return winner_; }

    Player GetCurrentPlayer() const { return currentPlayer_; }

    bool IsVsComputer() const { return vsComputer_; }

private:
    void ApplyMove(int col, Player player);
    void NextTurn();

    Board board_;
    GameState state_ = GameState::ModeSelect;

    bool vsComputer_ = false;
    Player currentPlayer_ = Player::Human;

    Outcome outcome_ = Outcome::Playing;
    std::optional<Player> winner_;
};
