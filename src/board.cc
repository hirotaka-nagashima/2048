//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------

#include "board.h"
#include <cmath>      // log2()
#include <ctime>      // time()
#include <cstdlib>    // srand(), rand()
#include <algorithm>  // std::max()
#include "sdl_option.h"

const int Board::kDirections[4] = {-kSize, kSize, -1, 1};
// http://gaming.stackexchange.com/questions/170665/
const double Board::kProbability4Panel = 0.1;

void Board::Initialize() {
  // Fill board with 0 and set sentinels.
  for (int i = 0; i < kBoardSize; ++i) {
    board_[i] = 0;
    if (i / kSize == 0 || i / kSize == kSize - 1 ||
        i % kSize == 0 || i % kSize == kSize - 1) {
      board_[i] = kSentinel;
    }
  }

  // Add first 2 panels randomly.
  srand(static_cast<unsigned int>(time(NULL)));
  AddPanel();
  AddPanel();
}

void Board::AddPanel() {
  if (IsFull())
    return;

  // Get a empty position of the board randomly.
  int random_id = 0;
  do {
    int x = rand() % kRealSize;
    int y = rand() % kRealSize;
    random_id = GetId(x, y);
  } while (board_[random_id] != 0);

  // Determine a number to add.
  bool is_next_4 = rand() % 100 < kProbability4Panel * 100;
  board_[random_id] = is_next_4 ? 2 : 1;
}

int Board::MovePanels(Direction direction) {
  int score = 0;
  int merged_point = 0;  // To merge each panels once.

  for (int i = GetId(0, 0);
       i <= GetId(kRealSize - 1, kRealSize - 1);
       IncreaseCounter(&i)) {
    // Calculate an origin corresponding to the given direction.
    int src = (direction == kUp || direction == kLeft) ?
        i : kBoardSize - 1 - i;

    if (board_[src] == 0)
      continue;

    // Search for a merged position.
    int dest;
    for (dest = src + kDirections[direction];
         board_[dest] == 0;
         dest += kDirections[direction]);

    bool is_mergeable = board_[dest] == board_[src];
    bool is_there_empty = board_[dest - kDirections[direction]] == 0;
    if (is_mergeable && dest != merged_point) {
      // If there is a same number panel
      // in the direction of "direction", merge panels.
      ++board_[dest];
      board_[src] = 0;
      score += 1 << board_[dest];
      merged_point = dest;
    } else if (is_there_empty) {
      // If there is an empty place in the direction of "direction",
      // move the panel there.
      board_[dest - kDirections[direction]] = board_[src];
      board_[src] = 0;
    }
  }

  return score;
}

bool Board::IsMovable(Direction direction) const {
  for (int i = GetId(0, 0);
       i <= GetId(kRealSize - 1, kRealSize - 1);
       IncreaseCounter(&i)) {
    if (board_[i] == 0)
      continue;

    // Search for a merged position.
    int dest;
    for (dest = i + kDirections[direction];
         board_[dest] == 0;
         dest += kDirections[direction]);

    // Check movability.
    bool is_there_empty = board_[dest - kDirections[direction]] == 0;
    bool is_mergeable = board_[dest] == board_[i];
    if (is_there_empty || is_mergeable)
      return true;
  }

  return false;
}

bool Board::IsGameOver() const {
  if (!IsFull())
    return false;
  if (IsMovable(kUp) || IsMovable(kDown) ||
      IsMovable(kLeft) || IsMovable(kRight)) {
    return false;
  }
  return true;
}

void Board::Draw() const {
  for (int i = GetId(0, 0);
       i <= GetId(kRealSize - 1, kRealSize - 1);
       IncreaseCounter(&i)) {
    if (board_[i] == 0)
      continue;

    int x = i % kSize - 1;
    int y = i / kSize - 1;
    SDLOption::DrawGraph(SDLOption::image_panels,
                         x * 94 + 17, y * 93 + 121,
                         board_[i] - 1, 83, 82);
  }
}

int Board::GetEvaluationValue() const {
  double board_evaluation_value = GetBoardEvaluationValue();
  double smoothness = GetSmoothness();
  double monotonicity = GetMonotonicity();
  double num_empty = CountEmptyCells();
  double max = GetMaxValue();
  double evaluation_value =
      board_evaluation_value * 4 +
      smoothness * 0.1 +
      monotonicity * 2.0 +
      log2(num_empty + 0.1) * 2.7 +
      max * 1.0;
  return static_cast<int>(evaluation_value);
}

int Board::GetMaxValue() const {
  int max_value = 0;
  for (int i = GetId(0, 0);
       i <= GetId(kRealSize - 1, kRealSize - 1);
       IncreaseCounter(&i)) {
    if (max_value < board_[i])
      max_value = board_[i];
  }
  return max_value;
}

bool Board::IsFull() const {
  for (int i = GetId(0, 0);
       i <= GetId(kRealSize - 1, kRealSize - 1);
       IncreaseCounter(&i)) {
    if (board_[i] == 0)
      return false;
  }
  return true;
}

int Board::GetId(int x, int y) const {
  int id_x = x + 1;  // Shift 1 for a sentinel.
  int id_y = y + 1;  // Shift 1 for a sentinel.
  return kSize * id_y + id_x;
}

bool Board::IsRightEndOfBoard(int id) const {
  return id % kSize == kRealSize;
}

void Board::IncreaseCounter(int *counter) const {
  if (IsRightEndOfBoard(*counter))
    *counter += 2 + 1;
  else
    *counter += 1;
}

int Board::GetBoardEvaluationValue() const {
  int evaluation_value = 0;
  int counter = kRealSize;
  for (int i = GetId(0, 0);
       i <= GetId(kRealSize - 2, 0);
       ++i, --counter) {
    if (board_[i + 1] <= board_[i])
      evaluation_value += (1 << board_[i]) * counter;
  }
  return evaluation_value;
}

int Board::GetSmoothness() const {
  int smoothness = 0;
  for (int i = GetId(0, 0);
       i <= GetId(kRealSize - 1, kRealSize - 1);
       IncreaseCounter(&i)) {
    smoothness -= abs(board_[i] - GetValueAdjacent(i, kUp));
    smoothness -= abs(board_[i] - GetValueAdjacent(i, kLeft));
  }
  return smoothness;
}

int Board::GetValueAdjacent(int position, Direction direction) const {
  int i;
  for (i = position + kDirections[direction];
       board_[i] == 0;
       i += kDirections[direction]);
  return (board_[i] != kSentinel) ? board_[i] : board_[position];
}

int Board::CountEmptyCells() const {
  int counter = 0;
  for (int i = GetId(0, 0);
       i <= GetId(kRealSize - 1, kRealSize - 1);
       IncreaseCounter(&i)) {
    if (board_[i] == 0)
      counter++;
  }
  return counter;
}

int Board::GetMonotonicity() const {
  int monotonicity[2] = {0};

  // Monotonicities of rows of the board.
  for (int x = 0; x < kRealSize - 1; ++x) {
    for (int y = 0; y < kRealSize; ++y) {
      int current = board_[GetId(x, y)];
      int next = board_[GetId(x + 1, y)];
      if (next < current)
        monotonicity[0] -= current - next;
      else if (current < next)
        monotonicity[1] -= next - current;
    }
  }

  return std::max(monotonicity[0], monotonicity[1]);
}