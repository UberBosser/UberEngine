#ifndef UE_PHYSICS_H
#define UE_PHYSICS_H
#include <Box2D/Box2D.h>
#include "UE_core.h"


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


class PhysicsGameManager : public GameManager {
    // For physics enabled games.
    public:
        PhysicsGameManager();
        PhysicsGameManager(const char* t, int w, int h, Uint32 f);

        void loop();
        
        ~PhysicsGameManager();

    private:
        ContactListener contactListener;
};

#endif
