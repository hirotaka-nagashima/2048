//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef GAMES_2048_GAME_H_
#define GAMES_2048_GAME_H_

#include "ai.h"
#include "board.h"

class Game {
public:
  struct Result {
    int score;
    int max_panel;
    double num_moves_per_sec;
  };

  Game();

  void Initialize();
  void Play(bool is_auto_mode);
  Result PlayAutoForAnalysis();

private:
  void DisplayScreen() const;
  void DisplayGameOver() const;
  Board::Direction InputDirection() const;

  int score_;
  Board board_;
  Ai ai_;
};

#endif  // GAMES_2048_GAME_H_