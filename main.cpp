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
        Game() : GameManager(){
            camera = new GameCamera(getScreenWidth(), getScreenHeight());
            group.add(new Player(renderer, 50, 50, "Assets/player.png")); 
            group.add(new Text(renderer, 200, 100, "Ceci n'est pas un test.", 42, "Assets/Roboto.ttf"));
            camera->setTarget(group.gameObjects[0]);
            group.add(camera);
        }

        virtual void update() {
            group.update();
        }

        virtual void draw() {
            group.draw(camera->getPosX(), camera->getPosY());
        }

    private:
        ObjectGroup group;
        GameCamera *camera;
};


int main() {
    Game game;
    game.loop();
}
