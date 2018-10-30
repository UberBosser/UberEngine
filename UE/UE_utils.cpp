#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "UE_core.h"
#include "UE_utils.h"


GameCamera::GameCamera(GameInfo* g, int w, int h) : SpriteObject(g) {
    rect.w = w;
    rect.h = h;
    maxBounds = false;
}

void GameCamera::setTarget(SpriteObject* t) {
    target = t;
}

void GameCamera::setSize(int w, int h) {
    rect.w = w;
    rect.h = h;
}

void GameCamera::setMaximumBounds(int w, int h) {
    maxBounds = true;
    maxW = w;
    maxH = h;
}

void GameCamera::update() {
    SDL_Rect targetRect = *target->getRect();
    rect.x = (targetRect.x + targetRect.w/2) - rect.w/2;
    rect.y = (targetRect.y + targetRect.h/2) - rect.h/2; 
    if (maxBounds) {
        if (rect.x < 0) {
            rect.x = 0;
        }
        if (rect.x > maxW - rect.w) {
            rect.x = maxW - rect.w;
        }
        if (rect.y < 0) {
            rect.y = 0; 
        }
        if (rect.y > maxH - rect.h) {
            rect.y = maxH - rect.h;
        }
    }
}


FontObject::FontObject(GameInfo* g) : SpriteObject(g) {
    color = {255, 255, 255, 255};
}

FontObject::FontObject(GameInfo* g, SDL_Color c) : SpriteObject(g) {
    color = c;
}

void FontObject::loadFont(int x, int y, std::string t, int s, const char* f) {
    font = TTF_OpenFont(f, s);
    if (!font) {
        printf("Couldn't open font: %s\n", TTF_GetError());
    }
    surface = TTF_RenderText_Blended(font, t.c_str(), color);
    rect = surface->clip_rect;
    dRect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
    SDL_FreeSurface(surface);
}

void FontObject::updateText(std::string t) {
    surface = TTF_RenderText_Blended(font, t.c_str(), color);
    int x = rect.x;
    int y = rect.y;
    rect = surface->clip_rect;
    dRect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
    SDL_FreeSurface(surface);
}
