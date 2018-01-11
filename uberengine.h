#ifndef UBERENGINE
#define UBERENGINE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>


class GameObject {
    public:
        SDL_Rect rect;
        GameObject();
        GameObject(SDL_Renderer *r);
        bool operator== (const GameObject &gameObject) const;
        void createSurface(int x, int y, int w, int h);
        void createSurface(int x, int y, int w, int h, Uint32 c);
        void createSurface(int x, int y, const char img[]);
        virtual void start();
        virtual void update();
        virtual void draw();

    private:
        SDL_Renderer *renderer;
        SDL_Surface *surface;
        SDL_Texture *texture; 
};


class ObjectGroup {
    public:
        ObjectGroup();
        std::vector <GameObject*> gameObjects;
        int gameObjectSize;
        void add(GameObject *g);
        void remove(GameObject g);
        void update();
        void draw();
};


class GameManager {
    public:
        GameManager();
        GameManager(int x, int y);
        virtual void events();
        virtual void update();
        virtual void draw();
        void capFps();
        void loop();
    
    protected:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Surface *screen;

    private: 
        Uint32 startTick;
        SDL_Event event;
        bool quit;
};

#endif
