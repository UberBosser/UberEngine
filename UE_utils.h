#ifndef UE_UTILS_H
#define UE_UTILS_H
#include "UE_core.h"

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
