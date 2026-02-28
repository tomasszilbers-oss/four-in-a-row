#pragma once
#include "Board.h"
#include "Rules.h"
#include "GameState.h"
#include "PlayerInfo.h"
#include <string>

class GameController
{
public:
    GameController();

    void NewGame(bool vsComputer,
                 const std::string& name1,
                 const std::string& name2,
                 bool firstPlayerStarts);

    // Called by UI when user selected a column.
    void OnHumanChooseColumn(int col);

    void ApplyMoveFromUI(int col);

    const Board& GetBoard() const { return board_; }
    GameState GetState() const { return state_; }

    Outcome GetOutcome() const { return outcome_; }
    std::optional<int> GetWinnerIndex() const { return winnerIndex_; }

    const PlayerInfo& GetCurrentPlayerInfo() const { return players_[currentPlayerIndex_]; }
    const PlayerInfo& GetPlayerInfo(int index) const { return players_[index]; }
    bool GetFirstPlayerStarts() const { return firstPlayerStarts_; }
    bool IsVsComputer() const { return vsComputer_; }
    int GetCurrentPlayerIndex() const { return currentPlayerIndex_; }

private:
    void ApplyMove(int col, int playerIndex);
    void NextTurn();

    Board board_;
    GameState state_ = GameState::ModeSelect;

    PlayerInfo players_[2];
    int currentPlayerIndex_ = 0;
    bool vsComputer_ = false;
    bool firstPlayerStarts_ = true;

    Outcome outcome_ = Outcome::Playing;
    std::optional<int> winnerIndex_;
};
