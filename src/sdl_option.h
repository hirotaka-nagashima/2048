//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef GAMES_2048_SDL_OPTION_H_
#define GAMES_2048_SDL_OPTION_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

namespace SDLOption {

extern SDL_Surface *video_surface;
extern SDL_Surface *image_board, *image_panels, *image_gameover;
extern TTF_Font *font;

void SDLInitialize();
void SDLFinalize();

void Sleep(const int duration);
void ClearScreen();

// Images must be 4 per row.
void DrawGraph(SDL_Surface *image, int dest_x, int dest_y,
                int image_id = 0,
                int image_width = 0, int image_height = 0);

void DrawString(const char *text, int dest_x, int dest_y,
                SDL_Color color = {0xff, 0xff, 0xff});
void CheckClose();
void WaitEnterKey();

};

#endif  // GAMES_2048_SDL_OPTION_H_