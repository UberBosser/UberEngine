#ifndef UE_CORE_H
#define UE_CORE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <Box2D/Box2D.h>
#include <vector>
#include "UE_keycodes.h"


#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#define GET_KEYS() SDL_GetKeyboardState(NULL)


// Game information, passed down to pretty much everything.
struct GameInfo {
    SDL_Renderer* renderer;
    b2World* world;
    SDL_Rect screenRect;
};

bool collideRect(SDL_Rect* a, SDL_Rect* b);


class SpriteObject {
    // No physics, simple sprite.
    public:
        SpriteObject(GameInfo* g); 
       
        void loadImage(int x, int y, const char* i, double s = 1);
        // SpriteSheet...
        void loadImage(int x, int y, int w, int h, const char* i, double s = 1);

        
        // If image is loaded this way don't forget to call SDL_FreeSurface(surface);.
        void loadImage(int x, int y, SDL_Surface* i);
        void loadImage(int x, int y, int w, int h, SDL_Surface* i);

        SDL_Rect* getRect();
        int getPosX();
        int getPosY();

        // Change frame of animation.
        void changeFrame(int i);
        
        // Flip sprite.
        void flip(SDL_RendererFlip f);

        virtual void update();

        virtual void draw();
        void drawTexture();

        // Camera/Offset 
        void drawTexture(SpriteObject* c); 
        virtual void draw(SpriteObject* c);

        virtual ~SpriteObject();

    protected:
        GameInfo* gameInfo;

        SDL_Rect rect;
        SDL_Rect dRect;
        float angle;

        SDL_Surface* surface;
        SDL_Texture* texture;

        SDL_Point pivot;

    private: 
        SDL_RendererFlip flipped;
        SDL_Rect offsetRect;

        std::vector <SDL_Rect> frames;
};


class GameManager {
    // Includes window, screen creation and game loop.
    public:
        GameManager();
        GameManager(const char* t, int w, int h, Uint32 f);

        GameInfo* getGameInfo();
        int getScreenWidth();
        int getScreenHeight();

        void setQuit(bool q);

        virtual void events();
        virtual void update();
        virtual void draw();
        void capFps();
        virtual void loop();

        ~GameManager();
    
    protected: 
        SDL_Window* window;
        SDL_Surface* screen;
        GameInfo gameInfo;
        Uint32 startTick;
        SDL_Event event;
        bool quit;
};

#endif
