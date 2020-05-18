//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------

#include "ai.h"
#include <climits>  // INT_MIN

const int Ai::kNumSimulation = 6;

Board::Direction Ai::GetBestDirection(const Board &board) const {
  Board::Direction best_direction;
  int best_evaluate_value = INT_MIN;
  for (int i = 0; i < 4; ++i) {  // 4 directions.
    Board::Direction direction = static_cast<Board::Direction>(i);

    // The current direction is unavailable.
    if (!board.IsMovable(direction))
      continue;

    // Simulate all patterns and get the evaluation value of
    // the board moved in the direction of current.
    int temp_evaluation_value =
        SimulateNextBoard(board, direction, kNumSimulation);
    if (best_evaluate_value < temp_evaluation_value) {
      best_evaluate_value = temp_evaluation_value;
      best_direction = direction;
    }
  }
  return best_direction;
}

int Ai::SimulateNextBoard(const Board &board,
                          Board::Direction direction,
                          int remaining_times) const {
  // Simulate a next board and calculate its evaluation value.
  Board next_board = board;
  int evaluation_value = (
      next_board.MovePanels(direction) +
      next_board.GetEvaluationValue()) * remaining_times;
  next_board.AddPanel();

  // If this node is a leaf.
  if (remaining_times <= 1)
    return evaluation_value;

  // Simulate next recursively.
  int best_evaluation_value = INT_MIN;
  for (int i = 0; i < 4; ++i) {  // 4 directions.
    Board::Direction direction = static_cast<Board::Direction>(i);

    // The current direction is unavailable.
    if (!next_board.IsMovable(direction))
      continue;

    // Simulate next and get the evaluation value of
    // the board moved in the direction of current.
    int temp_evaluation_value =
        SimulateNextBoard(next_board, direction, remaining_times - 1);
    if (best_evaluation_value < temp_evaluation_value)
      best_evaluation_value = temp_evaluation_value;
  }

  evaluation_value += best_evaluation_value;
  return evaluation_value;
}