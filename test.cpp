#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include "uberengine.h"
#include <time.h>

class Meteor : public SpriteObject {
    public:
        Meteor(GameObject* o, int x) : SpriteObject(o) {
            createSurface(x, -32, "Assets/meteor.png");
            health = 2;
        }
        void update() {
            rect.y += 3;
            angle += 2;
            if (rect.y > 256) {
                rect.y = -32;
            }
        }
        int* removeHealth() {
            health--; 
            return &health;
        }
    private:
        int health;
};


class Bullet : public SpriteObject {
    public:
        Bullet(GameObject* o, GameObjects <Meteor>* m, GameObjects <Bullet>* b, int x) : SpriteObject(o) {
            createSurface(x, 210, 2, 2);
            bullets = b;
            meteors = m;
        }
        void update() {
            rect.y -= 3;
            if (rect.y + rect.h < 40) {
                bullets->remove(this);
            }
            for (int i = 0; i < meteors->getObjectsSize(); i ++) {
                GameObject* o = meteors->get(i);
                if (this->collideRect(*o)) {
                    if (meteors->get(i)->removeHealth() == 0) {
                        meteors->remove(meteors->get(i));
                    }
                }
            }
        }

    private:
        GameObjects <Bullet>* bullets;
        GameObjects <Meteor>* meteors;
};


class Player : public SpriteObject {
    public:
        Player(GameManager* o, GameObjects <Meteor>* m) : SpriteObject(o) {
            createSurface(116, 210, "Assets/ship.png");
            meteors = m;
            gameManager = o;
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
                bullets.add(new Bullet(this, meteors, &bullets, rect.x + 11));
            }
            bullets.update();
            for (int i = 0; i < meteors->getObjectsSize(); i ++) {
                GameObject* o = meteors->get(i);
                if (this->collideRect(*o)) {
                    gameManager->setQuit(true);
                }
            }
        }
        void draw() {
            drawTexture();
            bullets.draw();
        }

    private:
        GameObjects <Bullet> bullets;
        GameObjects <Meteor>* meteors;
        GameManager* gameManager;
};


class Game : public GameManager {
    public:
        Game() : GameManager("NewGame", 256, 256, 0) { 
            player = new Player(this, &meteors);
            meteors.add(new Meteor(this, 30));
        }
        void update() {
            player->update();
            meteors.update();
            
        }
        void draw() {
            player->draw();
            meteors.draw();
        }
        ~Game() {
            delete player;
        }
    private:
        Player* player;
        GameObjects <Meteor> meteors;
};


int main() {
    Game game;
    game.loop();
}
