#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include "uberengine.h"
#include <time.h>


class Bullet : public SpriteObject {
    public:
        Bullet(GameObject* o, GameObjects <Bullet>* b, int x) : SpriteObject(o) {
            createSurface(x, 210, 2, 2);
            bullets = b;
        }
        void update() {
            rect.y -= 3;
            if (rect.y + rect.h < 40) {
                bullets->remove(this);
            }
        }

    private:
        GameObjects <Bullet>* bullets;
};


class Player : public SpriteObject {
    public:
        Player(GameObject* o) : SpriteObject(o) {
            createSurface(116, 210, "Assets/ship.png");
        }
        void update() {
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            if (keys[SDL_SCANCODE_A]) {
                rect.x -= 3; 
            }
            if (keys[SDL_SCANCODE_D]) {
                rect.x += 3;
            }
            if (keys[SDL_SCANCODE_SPACE]) {
                bullets.add(new Bullet(this, &bullets, rect.x));
            }
            bullets.update();
        }
        void draw() {
            drawTexture();
            bullets.draw();
        }

    private:
        GameObjects <Bullet> bullets;
};


class Game : public GameManager {
    public:
        Game() : GameManager("NewGame", 256, 256, 0) { 
            player = new Player(this);
        }
        void update() {
            player->update();
        }
        void draw() {
            player->draw();
        }
        ~Game() {
            delete player;
        }
    private:
        Player* player;
};


int main() {
    Game game;
    game.loop();
}
