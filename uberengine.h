#ifndef UBERENGINE
#define UBERENGINE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>


class GameObject {
    public:
        bool operator== (const GameObject *gameObject) const; 
        virtual void update();
        virtual void draw();

    protected:
        SDL_Renderer *renderer;
        SDL_Surface *surface;
        SDL_Texture *texture; 
        double angle;
        SDL_Point pivot;
        SDL_Rect rect;  
        SDL_RendererFlip flip;
};


class SpriteObject : public GameObject {
    public:
        SpriteObject(SDL_Renderer *r); 

        void createSurface(int x, int y, int w, int h);
        void createSurface(int x, int y, int w, int h, Uint32 c);
        void createSurface(int x, int y, const char *i);

        virtual void update();
        virtual void draw();
};


class TextObject : public GameObject {
    public:
        TextObject(SDL_Renderer *r); 

        void createSurface(int x, int y, const char *t, int s, const char *f);

        void updateText(const char *t);

        virtual void update();
        virtual void draw();

    private:
        TTF_Font *font;
        SDL_Color color;
        char *text;

};


class ObjectGroup {
    public:
        ObjectGroup();
        std::vector <GameObject*> gameObjects;
        int gameObjectSize;
        void add(GameObject *g);
        void remove(GameObject *g);
        void update();
        void draw();
};


class GameManager {
    public:
        GameManager();
        GameManager(int x, int y);
        
        void changeIcon(const char *f);
        void changeTitle(const char *t);

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
