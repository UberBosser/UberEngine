#ifndef UBERENGINE_H
#define UBERENGINE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <Box2D/Box2D.h>
#include <vector>


#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f


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
       
        void loadImage(int x, int y, const char* i);
        // SpriteSheet...
        void loadImage(int x, int y, int w, int h, const char* i);
        
        // If image is loaded this way don't forget to call SDL_FreeSurface(surface);.
        void loadImage(int x, int y, SDL_Surface* i);
        void loadImage(int x, int y, int w, int h, SDL_Surface* i);

        SDL_Rect* getRect();
        int getPosX();
        int getPosY();

        // Change frame of animation.
        void changeFrame(int i);        

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

    private:
        SDL_Surface* surface;
        SDL_Texture* texture;
        SDL_Point pivot;
        SDL_RendererFlip flip;
        SDL_Rect offsetRect;

        std::vector <SDL_Rect> frames;
};


class RigidBody : public SpriteObject {
    public:
        RigidBody(GameInfo* g);

        virtual void collisionStart();
        virtual void collisionEnd();

        virtual ~RigidBody();

    protected:
        b2Body* body;
};


class StaticBody : public RigidBody {
    // Doesn't move unless told to.
    public:
        StaticBody(GameInfo* g);

        // Creates a box fixture, using the loaded image size.
        void boxFixture();
        // Creates a circleFixture, using the loaded image size.
        void circleFixture();

        void setPosition(float x, float y);
};


class DynamicBody : public RigidBody {
    // Reacts to physics.
    public:
        DynamicBody(GameInfo* g);

        // Creates a box fixture, using the loaded image size.
        void boxFixture();
        // Creates a circleFixture, using the loaded image size.
        void circleFixture();

        void setPosition(float x, float y);
        
        b2Vec2 getVelocity();
        void setVelocity(int x, int y);

        virtual void update();
        void updatePhysics();
};


class ContactListener : public b2ContactListener {
    // Call RigidBody's collisionStart().
    void BeginContact(b2Contact* contact);
    
    // Call RigidBody's collisionEnd().
    void EndContact(b2Contact* contact);
};


template <class object>
class GameObjects {
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
        virtual void draw(SpriteObject* r) {
            for (int i = 0; i < gameObjectsSize; i++) {
                gameObjects[i]->draw(r);
            }
        }

        ~GameObjects() {}

    private:
        std::vector <object*> gameObjects;
        int gameObjectsSize;
};


class GameCamera : public SpriteObject {
    // GameCamera for when you want to follow a GameObject around.
    public:
       GameCamera(GameInfo* g, int w, int h);

       void setTarget(SpriteObject* t);
       void setSize(int w, int h);
       void setMaximumBounds(int w, int h);

       virtual void update();

    private:
       SpriteObject* target;
       bool maxBounds;
       int maxW, maxH;
};


class SoundManager {
    public:
        SoundManager(const char* d);
        SoundManager(const char* d, const int v);
        void play();
        void play(int i);
        ~SoundManager();

    private:
        Mix_Chunk* sound;
};


class MusicManager {
    public:
        MusicManager(const char* d);
        void play();
        void play(int t);
        ~MusicManager();

    private:
        Mix_Music *music;
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
        void loop();

        ~GameManager();
    
    protected: 
        SDL_Window* window;
        SDL_Surface* screen;

    private:
        GameInfo gameInfo;
        Uint32 startTick;
        SDL_Event event;
        bool quit;

        ContactListener contactListener;
};

#endif
