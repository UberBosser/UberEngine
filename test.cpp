#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "uberengine.h"
#include <time.h>


class Meteor : public SpriteObject {
    public:
        Meteor(GameObject* o, GameObjects <Meteor>* m, int x) : SpriteObject(o) {
            createSurface(x, -32, "Assets/meteor.png");
            meteors = m;
            speed = rand() % 4 + 1;
        }
        void update() {
            rect.y += speed;
            angle += 2;
            if (rect.y > 256) {
                meteors->remove(this);
            }
        } 

    private:
        GameObjects <Meteor>* meteors;
        int speed;
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
            if (rect.y + rect.h < 0) {
                bullets->remove(this);
            }
            for (int i = 0; i < meteors->getObjectsSize(); i ++) {
                GameObject* o = meteors->get(i);
                if (this->collideRect(*o)) {
                    bullets->remove(this);
                    meteors->remove(meteors->get(i));
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
            createSurface(116, 210, 24, 28, "Assets/ship.png");
            // shootSound = new SoundManager("Assets/laser.wav");
            meteors = m;
            gameManager = o;
            shotSpawn = 20;
        }
        void update() {
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            if (keys[SDL_SCANCODE_A]) {
                rect.x -= 3; 
            }
            if (keys[SDL_SCANCODE_D]) {
                rect.x += 3;
            }
            if (rect.x < -34) {
                rect.x = 290;
            }
            if (rect.x > 290) {
                rect.x = -34; 
            }
            if (shotSpawn == 20) {
                changeFrame(1);
                if (keys[SDL_SCANCODE_SPACE]) {
                    bullets.add(new Bullet(this, meteors, &bullets, rect.x + 11));
                    // shootSound->play();
                    shotSpawn = 0;
                }
            } else if (shotSpawn < 20) {
                shotSpawn++;
                changeFrame(0);
            }
            bullets.update(); 
        }
        void draw() {
            drawTexture();
            bullets.draw();
        }

    private:
        GameObjects <Bullet> bullets;
        GameObjects <Meteor>* meteors;
        // SoundManager *shootSound;
        GameManager* gameManager;
        int shotSpawn;
};

class Background {
    public:
        Background(GameObject *o) {
            backgroundOne = new SpriteObject(o);
            backgroundTwo = new SpriteObject(o);

            backgroundOne->createSurface(0, 0, "Assets/background1.png");
            backgroundTwo->createSurface(0, -256, "Assets/background2.png");
        }
        void update() {
            backgroundOne->changePosition(0, 1);
            backgroundTwo->changePosition(0, 1);
            if (*backgroundOne->getPosY() == 256) {
                backgroundOne->changePosition(0, -512);
            }
            if (*backgroundTwo->getPosY() == 256) {
                backgroundTwo->changePosition(0, -512);
            }
        }

        void draw() {
            backgroundOne->draw();
            backgroundTwo->draw();
        }

        ~Background() {
            delete backgroundOne;
            delete backgroundTwo; 
        }

    private:
        SpriteObject *backgroundOne;
        SpriteObject *backgroundTwo;
};

class Game : public GameManager {
    public:
        Game() : GameManager("Meteor Shooter", 256, 256, 0) { 
            background = new Background(this);
            player = new Player(this, &meteors);
            music = new MusicManager("Assets/music.wav");
            music->play();
            srand(time(NULL));
            meteorSpawn = 0;
        }
        void update() {
            background->update();
            player->update();
            meteors.update();
            for (int i = 0; i < meteors.getObjectsSize(); i ++) {
                GameObject* o = meteors.get(i);
                if (player->collideRect(*o)) {
                    setQuit(true);
                }
            }
            if (meteorSpawn % 20 == 0) {
                meteors.add(new Meteor(this, &meteors, rand() % 256 - 16)); 
                meteorSpawn = 0;
            }
            meteorSpawn++;
        }
        void draw() {
            background->draw();
            player->draw();
            meteors.draw();
        }
        ~Game() {
            delete player;
            delete music;
            delete background;
        }

    private:
        Background* background;
        Player* player;
        GameObjects <Meteor> meteors;
        MusicManager *music;
        int meteorSpawn;
};


int main() {
    Game game;
    game.loop();
}
