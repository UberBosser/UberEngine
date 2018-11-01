#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "UE_core.h"


bool collideRect(SDL_Rect* a, SDL_Rect* b) {
    if (a->y + a->h <= b->y) {
        return false;
    }
    if (a->y >= b->y + b->h) {
        return false; 
    }
    if (a->x + a->w <= b->x) {
        return false;
    }
    if (a->x >= b->x + b->w) {
        return false; 
    }
    return true;
}


SpriteObject::SpriteObject(GameInfo* g) {
    gameInfo = g;
    flipped = SDL_FLIP_NONE;
    angle = 0;
}

void SpriteObject::loadImage(int x, int y, const char* i, double s) {
    surface = IMG_Load(i);
    if (!surface) {
        printf("Couldn't load image: %s\n", IMG_GetError());
    }
    rect = surface->clip_rect;
    rect.w = rect.w * s;
    rect.h = rect.h * s;
    dRect = rect; 
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2; 
//    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
    SDL_FreeSurface(surface);
}

void SpriteObject::loadImage(int x, int y, int w, int h, const char* i, double s) {
    surface = IMG_Load(i);
    if (!surface) {
        printf("Couldn't load image: %s\n", IMG_GetError());
    }
    rect = surface->clip_rect;
    for (int yy = 0; yy < rect.h/h; yy++) {
        for (int xx = 0; xx < rect.w/w; xx++) {
            SDL_Rect r = {xx * w, yy * h, w, h};
            frames.push_back(r);
        }
    }
    dRect = frames[0];
    rect.h = h * s;
    rect.w = w * s;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
//    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
    SDL_FreeSurface(surface);
}

void SpriteObject::loadImage(int x, int y, SDL_Surface* i) {
    surface = i;
    if (!surface) {
        printf("Couldn't load image: %s\n", IMG_GetError());
    }
    rect = surface->clip_rect;
    dRect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
}

void SpriteObject::loadImage(int x, int y, int w, int h, SDL_Surface* i) {
    surface = i;
    if (!surface) {
        printf("Couldn't load image: %s\n", IMG_GetError());
    }
    rect = surface->clip_rect;
    for (int y = 0; y < rect.h/h; y++) {
        for (int x = 0; x < rect.w/w; x++) {
            SDL_Rect r = {x * w, y * h, w, h};
            frames.push_back(r);
        }
    }
    dRect = frames[0];
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;
    pivot.x = dRect.w/2;
    pivot.y = dRect.h/2;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
}

SDL_Rect* SpriteObject::getRect() {
    return &rect;
}

int SpriteObject::getPosX() {
    return rect.x;
}

int SpriteObject::getPosY() {
    return rect.y;
}

void SpriteObject::changeFrame(int i) { 
    dRect = frames[i];
}

void SpriteObject::flip(SDL_RendererFlip f) {
    flipped = f;
}

void SpriteObject::update() {}

void SpriteObject::draw() {
    drawTexture();
}

void SpriteObject::drawTexture() {
    if (collideRect(&rect, &gameInfo->screenRect)) {
        SDL_RenderCopyEx(gameInfo->renderer, texture, &dRect, &rect, angle, &pivot, flipped);
    }
}

void SpriteObject::draw(SpriteObject* c) {
    drawTexture(c);
}

void SpriteObject::drawTexture(SpriteObject* c) {
    // Set offset
    if (collideRect(&rect, c->getRect())) {
        offsetRect.x = rect.x - c->getPosX();
        offsetRect.y = rect.y - c->getPosY();
        offsetRect.w = rect.w;
        offsetRect.h = rect.h;
        SDL_RenderCopyEx(gameInfo->renderer, texture, &dRect, &offsetRect, angle, &pivot, flipped);
    }
}

SpriteObject::~SpriteObject() {
    SDL_DestroyTexture(texture);
}


GameManager::GameManager() {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();
    window = SDL_CreateWindow("UberEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1366, 768, SDL_WINDOW_FULLSCREEN_DESKTOP);
    gameInfo.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1536);
    gameInfo.screenRect.x = 0;
    gameInfo.screenRect.y = 0;
    SDL_GetRendererOutputSize(gameInfo.renderer, &gameInfo.screenRect.w, &gameInfo.screenRect.h);
    SDL_SetWindowIcon(window, IMG_Load("Assets/icon.png"));
}

GameManager::GameManager(const char* t, int w, int h, Uint32 f) {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG); 
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();
    window = SDL_CreateWindow(t, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, f);
    gameInfo.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1536);
    gameInfo.screenRect.x = 0;
    gameInfo.screenRect.y = 0;
    gameInfo.screenRect.w = w;
    gameInfo.screenRect.h = h;
    SDL_SetWindowIcon(window, IMG_Load("Assets/icon.png"));
}

GameInfo* GameManager::getGameInfo() {
    return &gameInfo;
}

int GameManager::getScreenWidth() {
    return gameInfo.screenRect.w;
}

int GameManager::getScreenHeight() {
    return gameInfo.screenRect.h;
}

void GameManager::setQuit(bool q) {
    quit = q;
}

void GameManager::events() {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true; 
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
    }
}

void GameManager::update() {}

void GameManager::draw() {}

void GameManager::capFps() {
    if ((1000/60) > SDL_GetTicks() - startTick) {
        SDL_Delay(1000/60 - (SDL_GetTicks() - startTick));
    }
}

void GameManager::loop() {
    while (!quit) {
        startTick = SDL_GetTicks();
        events();
        update();
        SDL_RenderClear(gameInfo.renderer);
        draw();
        SDL_RenderPresent(gameInfo.renderer);
        capFps();
    }
    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(gameInfo.renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

GameManager::~GameManager() {}
