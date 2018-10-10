#ifndef UE_UTILS_H
#define UE_UTILS_H
#include "UE_core.h"


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
    // GameCamera for when you want to follow a SpriteObject around.
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

#endif
