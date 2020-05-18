//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------

#include "game.h"
#include <cstdio>  // sprintf_s()
#include <ctime>   // clock()
#include "sdl_option.h"

Game::Game() : score_(0) {
}

void Game::Initialize() {
  score_ = 0;
  board_.Initialize();
}

void Game::Play(bool is_auto_mode) {
  DisplayScreen();
  while (!board_.IsGameOver()) {
    Board::Direction direction = is_auto_mode ?
        ai_.GetBestDirection(board_) : InputDirection();
    score_ += board_.MovePanels(direction);
    board_.AddPanel();
    DisplayScreen();
  }
  DisplayGameOver();
}

Game::Result Game::PlayAutoForAnalysis() {
  clock_t start = clock();

  // Play 2048 not displaying screen.
  int num_moves = 0;
  while (!board_.IsGameOver()) {
    Board::Direction direction = ai_.GetBestDirection(board_);
    score_ += board_.MovePanels(direction);
    board_.AddPanel();
    ++num_moves;
  }

  clock_t end = clock();
  double time = 1.0 * (end - start) / CLOCKS_PER_SEC;

  // Return its result.
  Result result;
  result.score = score_;
  result.max_panel = 1 << board_.GetMaxValue();
  result.num_moves_per_sec = 1.0 * num_moves / time;
  return result;
}

void Game::DisplayScreen() const {
  // Convert the score into string.
  char score_string[10];
  sprintf_s(score_string, "%8d", score_);

  // Draw the screen. ->
  SDLOption::ClearScreen();
  SDLOption::DrawGraph(SDLOption::image_board, 0, 0);  // Background.
  SDLOption::DrawString(score_string, 250, 70);        // Score.
  board_.Draw();                                       // Board.

  SDL_Flip(SDLOption::video_surface);
}

void Game::DisplayGameOver() const {
  // Convert the score into string.
  char score_string[10];
  sprintf_s(score_string, "%8d", score_);

  // Draw an overlay.
  SDLOption::DrawGraph(SDLOption::image_gameover, 0, 0);
  SDLOption::DrawString(score_string, 155, 295, {0x21, 0x21, 0x21});
  SDL_Flip(SDLOption::video_surface);

  SDLOption::WaitEnterKey();
}

Board::Direction Game::InputDirection() const {
  Board::Direction direction;
  do {
    SDL_WaitEvent(NULL);
    SDLOption::CheckClose();
    Uint8 *key = SDL_GetKeyState(NULL);
    direction = key[SDLK_UP]    ? Board::kUp :
                key[SDLK_DOWN]  ? Board::kDown :
                key[SDLK_LEFT]  ? Board::kLeft :
                key[SDLK_RIGHT] ? Board::kRight :
                                  Board::kNone;
  } while (direction == Board::kNone || !board_.IsMovable(direction));
  return direction;
}