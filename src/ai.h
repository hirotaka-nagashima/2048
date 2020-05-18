//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef GAMES_2048_AI_H_
#define GAMES_2048_AI_H_

#include "board.h"

class Ai {
public:
  static const int kNumSimulation;

  Board::Direction GetBestDirection(const Board &board) const;

private:
  int SimulateNextBoard(const Board &board,
                        Board::Direction direction,
                        int remaining_times) const;
};

#endif  // GAMES_2048_AI_H_