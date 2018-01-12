#ifndef UBERENGINE
#define UBERENGINE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>


class GameObject {
    public:
        bool operator== (const GameObject *gameObject) const; 

        bool collideRect(GameObject gameObject);

        int getPosX();
        int getPosY();
        int getWidth();
        int getHeight();

        SDL_Renderer* getRenderer();

        void addChild(GameObject *g);
        void removeChild(GameObject *g);
        GameObject* getChild(int i);
        bool hasChildren();

        virtual void update();
        virtual void updateChildren();
        virtual void draw();
        virtual void draw(int offsetX, int offsetY);
        virtual void drawChildren();
        virtual void drawChildren(int offsetX, int offsetY);

    protected:
        SDL_Renderer *renderer;
        SDL_Surface *surface;
        SDL_Texture *texture; 

        double angle;
        SDL_Point pivot;
        SDL_Rect rect;  
        SDL_RendererFlip flip;
        
        GameObject *parent;

        std::vector <GameObject*> children;
        int childrenSize;
};


class SpriteObject : public GameObject {
    public:
        SpriteObject(SDL_Renderer *r);
        SpriteObject(GameObject *p); 

        void createSurface(int x, int y, int w, int h);
        void createSurface(int x, int y, int w, int h, Uint32 c);
        void createSurface(int x, int y, const char *i);

        virtual void update();
        virtual void draw();
        virtual void draw(int offsetX, int offsetY);
};


class TextObject : public GameObject {
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


class GameCamera : public GameObject {
    public:
       GameCamera(int w, int h);

       void setTarget(GameObject *gameObject);
       void setSize(int w, int h);

       virtual void update();

    private:
       GameObject *target;
};


class GameManager {
    public:
        GameManager();
        GameManager(int x, int y);

        int getScreenWidth();
        int getScreenHeight();
        SDL_Renderer* getRenderer();

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
        int screenWidth, screenHeight;
};

#endif
