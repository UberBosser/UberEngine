#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <SDL2/SDL_image.h>
#include "uberengine.h"

class Player : public GameObject {
    public:
        Player(int x, int y, Uint32 c, SDL_Renderer *r) : GameObject(r){
            createSurface(x, y, 30, 30, c);
        }

        virtual void update() {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            if (keys[SDL_SCANCODE_LEFT]) {
                rect.x += -1;
            }
            if (keys[SDL_SCANCODE_RIGHT]) {
                rect.x += 1;
            }
        }
};

class Game : public GameManager {
    public:
        Game() : GameManager(){
            group.add(new Player(50, 50, SDL_MapRGB(screen->format, 255, 0, 0), renderer));
        }

        virtual void update() {
            group.update();
        }

        virtual void draw() {
            group.draw();
        }

    private:
        ObjectGroup group;
};

int main() {
    Game game;
    game.loop();
}
