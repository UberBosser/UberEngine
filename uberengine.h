#ifndef UBERENGINE_H
#define UBERENGINE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>


class GameObject {
    // The Base of any object in the engine.
    public:
        bool operator== (const GameObject *gameObject) const; 

        bool collideRect(GameObject gameObject);
        bool collideRectVector(std::vector <GameObject*> rects, int size);
        
        SDL_Rect* getRect();
        int* getPosX();
        int* getPosY();
        int* getWidth();
        int* getHeight();

        SDL_Renderer* getRenderer();

        virtual void update();
        void drawTexture();
        void drawTexture(int offsetX, int offsetY);
        virtual void draw();
        virtual void draw(int offsetX, int offsetY);

        virtual ~GameObject();

    protected:
        SDL_Renderer *renderer;
        SDL_Surface *surface;
        SDL_Texture *texture; 

        double angle;
        SDL_Point pivot;
        SDL_Rect rect;
        SDL_Rect offsetRect;
        bool collidable;
        SDL_RendererFlip flip;
        
        GameObject *parent;
};


class SpriteObject : public GameObject {
    // SpriteObjects are drawable Sprites, they can load images or make rectangles.
    public:
        SpriteObject(SDL_Renderer *r);
        SpriteObject(GameObject *p); 

        void createSurface(int x, int y, int w, int h);
        void createSurface(int x, int y, int w, int h, Uint32 c);
        void createSurface(int x, int y, const char *i);

        virtual void update();
        virtual void draw();
        // Offset is usually for the CameraObject
        virtual void draw(int offsetX, int offsetY);
};


class TextObject : public GameObject {
    // Kind of like the SpriteObject, but instead it loads a font to draw text.
    public:
        TextObject(SDL_Renderer *r);
        TextObject(GameObject *p);

        void createSurface(int x, int y, const char *t, int s, const char *f);

        void updateText(const char *t);

        virtual void update();
        virtual void draw();
        virtual void draw(int offsetX, int offsetY);

    private:
        TTF_Font *font;
        SDL_Color color;
        char *text;

};


template <class object>
class GameObjects : public GameObject {
    // A group of GameObjects and their children, provides an easy way to manage multiple GameObjects.
    public:
        GameObjects() {
            gameObjectsSize = 0;
        }
        void add(object* g) {
            gameObjects.push_back(g);
            gameObjectsSize = gameObjects.size();
        }
        void remove(object* g) {
            for (int i = 0; i < gameObjectsSize; i++) {
                if (gameObjects[i] == g) {
                    delete gameObjects[i];
                    gameObjects.erase(gameObjects.begin() + i); 
                    gameObjectsSize = gameObjects.size();
                }
            }
        }
        void clear() {
            for (int i = 0; i < gameObjectsSize; i++) {
                    delete gameObjects[i];
                    gameObjectsSize = gameObjects.size();
                }
                gameObjects.clear();
        }

        object* get(int i) {
            return gameObjects[i] ;
        }
        int getObjectsSize() {
            return gameObjectsSize;
        }

        virtual void update() {
            for (int i = 0; i < gameObjectsSize; i++) {
                gameObjects[i]->update();
            }
        }
        virtual void draw() {
            for (int i = 0; i < gameObjectsSize; i++) {
                gameObjects[i]->draw();
            }
        }
        virtual void draw(int offsetX, int offsetY) {
            for (int i = 0; i < gameObjectsSize; i++) {
                gameObjects[i]->draw(offsetX, offsetY);
            }
        }

        ~GameObjects() {
            
        }

    private:
        std::vector <object*> gameObjects;
        int gameObjectsSize;
};


class GameCamera : public GameObject {
    // GameCamera for when you want to follow a GameObject around.
    public:
       GameCamera(int w, int h);

       void setTarget(GameObject *gameObject);
       void setSize(int w, int h);
       void setMaximumBounds(int w, int h);

       virtual void update();

    private:
       GameObject *target;
       bool maxBounds;
       int maxW, maxH;
};


class SoundManager : public GameObject {
    public:
        SoundManager(const char *d);
        SoundManager(const char *d, const int v);
        void play();
        void play(int i);
        ~SoundManager();

    private:
        Mix_Chunk *sound;
};


class MusicManager : public GameObject {
    public:
        MusicManager(const char *d);
        void play();
        void play(int t);
        ~MusicManager();

    private:
        Mix_Music *music;
};


class GameManager : public GameObject {
    // Includes window, screen creation and game loop.
    public:
        GameManager();
        GameManager(const char* t, int w, int h, Uint32 f);

        int getScreenWidth();
        int getScreenHeight();

        void setQuit(bool q);

        virtual void events();
        virtual void update();
        virtual void draw();
        void capFps();
        void loop();
    
    protected:
        SDL_Window *window;
        SDL_Surface *screen;

    private: 
        Uint32 startTick;
        SDL_Event event;
        bool quit;
        int screenWidth, screenHeight;
};

#endif
