//-----------------------------------------------------------------------------
// Copyright (c) 2014 Hirotaka Nagashima. All rights reserved.
//-----------------------------------------------------------------------------

#include "sdl_option.h"
#include <cstdio>

namespace SDLOption {

SDL_Window *window;
SDL_Surface *video_surface;
SDL_Surface *image_board, *image_panels, *image_gameover;
TTF_Font *font;

void SDLInitialize() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "ERROR: %s\n", SDL_GetError());
    exit(-1);
  }
  if (TTF_Init() < 0) {
    fprintf(stderr, "ERROR: %s\n", TTF_GetError());
    SDL_Quit();
    exit(-1);
  }
  window = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            400, 500, 0);
  if (!window) {
    fprintf(stderr, "ERROR: %s\n", SDL_GetError());
    TTF_Quit();
    SDL_Quit();
    exit(-1);
  }
  video_surface = SDL_GetWindowSurface(window);

  // Load images.
  image_board = IMG_Load("src/resources/board.jpg");
  image_panels = IMG_Load("src/resources/panels.jpg");
  image_gameover = IMG_Load("src/resources/gameover.png");
  if (!image_board || !image_panels || !image_gameover) {
    fprintf(stderr, "ERROR: %s\n", IMG_GetError());
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    exit(-1);
  }

  // Load a font.
  font = TTF_OpenFont("src/resources/font.ttf", 20);
  if (!font) {
    fprintf(stderr, "ERROR: %s\n", TTF_GetError());
    SDL_FreeSurface(image_gameover);
    SDL_FreeSurface(image_panels);
    SDL_FreeSurface(image_board);
    SDL_DestroyWindow(window);
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

  SDL_DestroyWindow(window);

  TTF_Quit();
  SDL_Quit();
}

void UpdateScreen() {
  SDL_UpdateWindowSurface(window);
}

void Sleep(int duration) {
  for (int i = 0; i < duration / 10; ++i) {
    SDL_Delay(10);
    CheckClose();
  }
}

void ClearScreen() {
  SDL_FillRect(video_surface, NULL, 0);
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
  SDL_Event event;
  while (true) {
    SDL_WaitEvent(&event);
    CheckClose();
    const Uint8 *key = SDL_GetKeyboardState(NULL);
    if (key[SDL_SCANCODE_RETURN] || key[SDL_SCANCODE_KP_ENTER])
      break;
  }
}

};  // namespace SDLOption