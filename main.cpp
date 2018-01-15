#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "uberengine.h"


class Player : public SpriteObject {
    public:
        Player(SDL_Renderer *r, int x, int y, const char *f) : SpriteObject(r) {
            createSurface(x, y, f);
        }

        virtual void update() {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            if (keys[SDL_SCANCODE_LEFT]) {
                rect.x += -3;
                angle += -3;
            }
            if (keys[SDL_SCANCODE_RIGHT]) {
                rect.x += 3;
                angle += 3;
            }
        }
};


class Text : public TextObject {
    public:
        Text(SDL_Renderer *r, int x, int y, const char *t, int s, const char *f) : TextObject(r) {
            createSurface(x, y, t, s, f); 
        }
};


class Game : public GameManager {
    public:
        Game() {
            camera = new GameCamera(getScreenWidth(), getScreenHeight());
            addChild(new Player(renderer, 50, 50, "Assets/player.png"));
            camera->setMaximumBounds(2000, getScreenHeight());
            addChild(new Text(renderer, 200, 100, "Ceci n'est pas un test. ", 42, "Assets/Roboto.ttf"));
            camera->setTarget(player);
        }

        virtual void update() {
            player->update();
            camera->update();
        }

        virtual void draw() {
            player->draw(*camera->getPosX(), *camera->getPosY());
            player->drawChildren(*camera->getPosX(),*camera->getPosY());
        }

    private:
        GameCamera *camera;
        Player *player;
};


int main() {
    Game game;
    game.loop();
}
