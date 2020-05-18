//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef GAMES_2048_BOARD_H_
#define GAMES_2048_BOARD_H_

class Board {
public:
  enum Panel { kSentinel = -1 };
  enum Direction { kUp, kDown, kLeft, kRight, kNone };

  static const int kRealSize = 4;
  static const int kSize = 1 + kRealSize + 1;
  static const int kBoardSize = kSize * kSize;
  static const int kDirections[4];

  void Initialize();
  void AddPanel();

  /// <summary>
  /// Move panels in the direction of given.
  /// </summary>
  /// <param name="direction">direction to move</param>
  /// <returns>added score</returns>
  int MovePanels(Direction direction);

  bool IsMovable(Direction direction) const;
  bool IsGameOver() const;
  void Draw() const;
  int GetEvaluationValue() const;
  int GetMaxValue() const;

private:
  static const double kProbability4Panel;

  bool IsFull() const;
  int GetId(int x, int y) const;
  bool IsRightEndOfBoard(int id) const;
  void IncreaseCounter(int *counter) const;

  // Evaluate a board.
  // http://rhysd.hatenablog.com/entry/2014/05/17/171817
  // http://stackoverflow.com/questions/22342854/
  int GetBoardEvaluationValue() const;
  int GetSmoothness() const;
  int GetValueAdjacent(int position, Direction direction) const;
  int CountEmptyCells() const;
  int GetMonotonicity() const;

  int board_[kBoardSize];
};

#endif  // GAMES_2048_BOARD_H_