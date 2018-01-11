#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "uberengine.h"


GameObject::GameObject() {}

GameObject::GameObject(SDL_Renderer *r) {
    renderer = r;
}

bool GameObject::operator==(const GameObject &gameObject) const {
    return (texture == gameObject.texture);
}

void GameObject::createSurface(int x, int y, int w, int h) {
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void GameObject::createSurface(int x, int y, int w, int h, Uint32 c) {
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    SDL_FillRect(surface, NULL, c);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); 
}

void GameObject::createSurface(int x, int y, const char img[]) {
    surface = IMG_Load(img);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void GameObject::start() {}

void GameObject::update() {}

void GameObject::draw() {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}


ObjectGroup::ObjectGroup() {}

void ObjectGroup::add(GameObject *g) {
    gameObjects.push_back(g);
    gameObjectSize = gameObjects.size();
}

void ObjectGroup::remove(GameObject g) {
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
    window = SDL_CreateWindow("uberengine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1366, 768, SDL_WINDOW_FULLSCREEN_DESKTOP);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
}

GameManager::GameManager(int x, int y) {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
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
