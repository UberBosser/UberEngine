#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "uberengine.h"


bool GameObject::operator==(const GameObject *gameObject) const {
    return (this == gameObject);
}

void GameObject::update() {}

void GameObject::draw() {}


SpriteObject::SpriteObject(SDL_Renderer *r) {
    renderer = r;
}

void SpriteObject::createSurface(int x, int y, int w, int h) {
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void SpriteObject::createSurface(int x, int y, int w, int h, Uint32 c) {
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    SDL_FillRect(surface, NULL, c);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); 
}

void SpriteObject::createSurface(int x, int y, const char *i) {
    surface = IMG_Load(i);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void SpriteObject::update() {}

void SpriteObject::draw() {
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &pivot, flip);
}


TextObject::TextObject(SDL_Renderer *r) {
    renderer = r;
    color.r = 255;
    color.g = 255;
    color.b = 255;
}

void TextObject::createSurface(int x, int y, const char *t, int s, const char *f) {
    font = TTF_OpenFont(f, 24);
    surface = TTF_RenderText_Blended(font, t, color);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void TextObject::updateText(const char *t) {
    surface = TTF_RenderText_Blended(font, t, color);
    rect = surface->clip_rect;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); 
}

void TextObject::update() {}

void TextObject::draw() {
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &pivot, flip);
}


ObjectGroup::ObjectGroup() {}

void ObjectGroup::add(GameObject *g) {
    gameObjects.push_back(g);
    gameObjectSize = gameObjects.size();
}

void ObjectGroup::remove(GameObject *g) {
    for (int i = 0; i < gameObjectSize; i++) {
        if (*gameObjects[i] == g) {
            gameObjects.erase(gameObjects.begin() + i);
            gameObjectSize = gameObjects.size();
        }
    }
}

void ObjectGroup::update() {
    for (int i = 0; i < gameObjectSize; i++) {
        gameObjects[i]->update();
    }
}

void ObjectGroup::draw() {
    for (int i = 0; i < gameObjectSize; i++) {
        gameObjects[i]->draw();
    }
}


GameManager::GameManager() {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("uberengine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1366, 768, SDL_WINDOW_FULLSCREEN_DESKTOP);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
}

GameManager::GameManager(int x, int y) {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("uberengine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_FULLSCREEN_DESKTOP);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
}

void GameManager::events() {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true; 
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
            quit = true;}
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
    while (quit == false) {
        startTick = SDL_GetTicks();
        events();
        update();
        SDL_RenderClear(renderer);
        draw();
        SDL_RenderPresent(renderer);
        capFps();
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
