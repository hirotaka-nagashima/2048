//-----------------------------------------------------------------------------
// Copyright (c) 2014 @hirodotexe. All rights reserved.
//-----------------------------------------------------------------------------

#include "sdl_option.h"
#include <cstdio>

namespace SDLOption {

SDL_Surface *video_surface;
SDL_Surface *image_board, *image_panels, *image_gameover;
TTF_Font *font;

void SDLInitialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "ERROR: %s\n", SDL_GetError());
    exit(-1);
  }
  if (TTF_Init() < 0) {
    fprintf(stderr, "ERROR: %s\n", TTF_GetError());
    SDL_Quit();
    exit(-1);
  }
  if (SDL_SetVideoMode(400, 500, 16, SDL_HWSURFACE) == NULL) {
    fprintf(stderr, "ERROR: %s\n", SDL_GetError());
    TTF_Quit();
    SDL_Quit();
    exit(-1);
  }
  SDL_WM_SetCaption("2048", "2048");
  video_surface = SDL_GetVideoSurface();

  // Load images.
  image_board = IMG_Load("resources/board.jpg");
  image_panels = IMG_Load("resources/panels.jpg");
  image_gameover = IMG_Load("resources/gameover.png");
  if (!image_board || !image_panels || !image_gameover) {
    fprintf(stderr, "ERROR: %s\n", IMG_GetError());
    SDL_FreeSurface(video_surface);
    TTF_Quit();
    SDL_Quit();
    exit(-1);
  }

  // Load a font.
  font = TTF_OpenFont("resources/font.ttf", 20);
  if (!font) {
    fprintf(stderr, "ERROR: %s\n", TTF_GetError());
    SDL_FreeSurface(image_gameover);
    SDL_FreeSurface(image_panels);
    SDL_FreeSurface(image_board);
    SDL_FreeSurface(video_surface);
    TTF_Quit();
    SDL_Quit();
    exit(-1);
  }
}

void SDLFinalize() {
  TTF_CloseFont(font);

  SDL_FreeSurface(image_gameover);
  SDL_FreeSurface(image_panels);
  SDL_FreeSurface(image_board);

  SDL_FreeSurface(video_surface);

  TTF_Quit();
  SDL_Quit();
}

void Sleep(int duration) {
  for (int i = 0; i < duration / 10; ++i) {
    SDL_Delay(10);
    CheckClose();
  }
}

void ClearScreen() {
  SDL_FillRect(video_surface, 0, 0);
}

void DrawGraph(SDL_Surface *image, int dest_x, int dest_y,
                int image_id,
                int image_width, int image_height) {
  SDL_Rect src, dest;
  src.x = (image_id % 4) * image_width;
  src.y = (image_id / 4) * image_height;
  src.w = (image_width == 0) ? image->w : image_width;
  src.h = (image_height == 0) ? image->h : image_height;
  dest.x = dest_x;
  dest.y = dest_y;
  SDL_BlitSurface(image, &src, video_surface, &dest);
}

void DrawString(const char *text, int dest_x, int dest_y,
                SDL_Color color) {
  SDL_Surface *temp_text = TTF_RenderUTF8_Blended(font, text, color);
  SDL_Rect src, dest;
  src.x = src.y = 0;
  src.w = temp_text->w;
  src.h = temp_text->h;
  dest.x = dest_x;
  dest.y = dest_y;
  SDL_BlitSurface(temp_text, &src, video_surface, &dest);
  SDL_FreeSurface(temp_text);
}

void CheckClose() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)  // Task was killed.
      exit(0);
  }
}

void WaitEnterKey() {
  while (true) {
    SDL_WaitEvent(NULL);
    CheckClose();
    Uint8 *key = SDL_GetKeyState(NULL);
    if (key[SDLK_RETURN])
      break;
  }
}

};  // namespace SDLOption