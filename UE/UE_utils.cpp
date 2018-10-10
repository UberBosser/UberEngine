#include <SDL2/SDL.h>
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
