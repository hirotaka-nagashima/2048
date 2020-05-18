//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------
// 2016/10/26: Reconstructed with C++.
// 2014/10/06: Added evaluation functions.
//   (100 samples, search depth 6, release mode, R731)
//   421 [moves/sec], 43962 (9812 ~ 80252) [points]
//   2048 85%
//   4096 40%
// 2014/04/23: Considered next panels.
//   2048 30%
// 2014/04/23: Adjusted weights of each evaluation value.
//   2048 10%
// 2014/04/22: Now AI can simulate next states recursively.
//   2048 2%
// 2014/04/21: Added the AI.
//   2048 0%
// 2014/04/20: Worked successfully with GUI, SDL.
// 2014/04/20: Worked successfully with CUI.
// 2014/04/20: Project was created.
//-----------------------------------------------------------------------------

#include <cstdio>
#include "game.h"
#include "sdl_option.h"

// NOTE: Comment out below to use SDL in VS2015 or later versions.
// https://stackoverflow.com/questions/30412951/
FILE _iob[] = {*stdin, *stdout, *stderr};
extern "C" FILE * __cdecl __iob_func() {
  return _iob;
}

void Play(bool is_auto_mode) {
  while (true) {
    Game game;
    game.Initialize();
    game.Play(is_auto_mode);
  }
}

void Analyze() {
  static const int kNumAnalysis = 100;
  static const char kLogFile[] = "log.csv";

  // Clear content of the log file.
  FILE *log;
  fopen_s(&log, kLogFile, "w");
  fclose(log);

  // Play game many times.
  for (int i = 0; i < kNumAnalysis; ++i) {
    Game game;
    game.Initialize();
    Game::Result result = game.PlayAutoForAnalysis();

    // Log the result.
    fopen_s(&log, kLogFile, "a");
    fprintf_s(log, "%d,%d,%f\n",
              result.score, result.max_panel, result.num_moves_per_sec);
    fclose(log);
  }
}

int main(int argc, char* argv[]) {
  SDLOption::SDLInitialize();
  Play(true);
  SDLOption::SDLFinalize();
  return 0;
}