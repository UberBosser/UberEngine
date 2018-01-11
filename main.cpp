#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <SDL2/SDL_image.h>
#include "uberengine.h"

class Player : public GameObject {
    Player(int x, int y, SDL_Renderer *r) : GameObject(r){
    
    }
};

int main() {
    GameManager gm;
    gm.loop();
}
