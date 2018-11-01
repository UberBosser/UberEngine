#include "UE_core.h"
#include "UE_physics.h"


RigidBody::RigidBody(GameInfo* g) : SpriteObject(g){
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.angle = 0.0f;
    body = gameInfo->world->CreateBody(&bodyDef);
    body->SetUserData(this);
}

void RigidBody::collisionStart() {}

void RigidBody::collisionEnd() {}

RigidBody::~RigidBody() { 
    body->GetWorld()->DestroyBody(body);
}


StaticBody::StaticBody(GameInfo* g) : RigidBody(g) {
   body->SetType(b2_staticBody);
}

void StaticBody::boxFixture() {
    b2PolygonShape boxShape;
    boxShape.SetAsBox((rect.w * 0.01f)/2.0f - 0.01f, (rect.h * 0.01f)/2.0f - 0.01f);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;

    body->CreateFixture(&boxFixtureDef);

    setPosition(rect.x * 0.01f, rect.y * 0.01f);
}

void StaticBody::circleFixture() {
    b2CircleShape circleShape;
    circleShape.m_radius = (rect.w/2 + rect.h/2)/2 * 0.01;

    b2FixtureDef circleFixtureDef;
    circleFixtureDef.shape = &circleShape;

    body->CreateFixture(&circleFixtureDef);
    
    setPosition(rect.x * 0.01f, rect.y * 0.01f);
}

void StaticBody::setPosition(float x, float y) {
    body->SetTransform(b2Vec2(x + (rect.w * 0.01f)/2.0f, y + (rect.h * 0.01f)/2.0f), body->GetAngle());
}


DynamicBody::DynamicBody(GameInfo* g) : RigidBody(g){
    body->SetType(b2_dynamicBody);    
}

void DynamicBody::boxFixture() {
    b2PolygonShape boxShape;
    boxShape.SetAsBox((rect.w * 0.01f)/2.0f - 0.01f, (rect.h * 0.01f)/2.0f - 0.01f);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    boxFixtureDef.restitution = 0.3;

    body->CreateFixture(&boxFixtureDef);
    
    setPosition(rect.x * 0.01f, rect.y * 0.01f); 
}

void DynamicBody::circleFixture() {
    b2CircleShape circleShape;
    circleShape.m_radius = (rect.w/2 + rect.h/2)/2 * 0.01;

    b2FixtureDef circleFixtureDef;
    circleFixtureDef.shape = &circleShape;
    circleFixtureDef.density = 1;
    circleFixtureDef.restitution = 0.3;

    body->CreateFixture(&circleFixtureDef);
    
    setPosition(rect.x * 0.01f, rect.y * 0.01f);
}

void DynamicBody::setDensity(float d) {
    b2Fixture* fixture = body->GetFixtureList();
    fixture->SetDensity(d);
}

void DynamicBody::setRestitution(float r) {
    b2Fixture* fixture = body->GetFixtureList();
    fixture->SetRestitution(r);
}

void DynamicBody::setFriction(float f) {
    b2Fixture* fixture = body->GetFixtureList();
    fixture->SetFriction(f);
}

void DynamicBody::setPosition(float x, float y) {
    body->SetTransform(b2Vec2(x + (rect.w * 0.01f)/2.0f, y + (rect.h * 0.01f)/2.0f), body->GetAngle());
}

void DynamicBody::setVelocity(float x, float y) {
    // Fix int to box2d meters.
    body->SetLinearVelocity(b2Vec2(x, y));
}

void DynamicBody::update() {
    updatePhysics();
}

void DynamicBody::updatePhysics() {
    b2Vec2 pos = body->GetPosition();
    rect.x = (pos.x - (rect.w * 0.01f)/2.0f) * 100;
    rect.y = (pos.y - (rect.h * 0.01f)/2.0f) * 100;
    angle = body->GetAngle() * RADTODEG;
}


void ContactListener::BeginContact(b2Contact* contact) {
    void* rigidBody = contact->GetFixtureA()->GetBody()->GetUserData();
    if (rigidBody) {
        static_cast<RigidBody*>(rigidBody)->collisionStart();
    }

    rigidBody = contact->GetFixtureB()->GetBody()->GetUserData();
    if (rigidBody) {
        static_cast<RigidBody*>(rigidBody)->collisionStart();
    }
}

void ContactListener::EndContact(b2Contact* contact) {
    void* rigidBody = contact->GetFixtureA()->GetBody()->GetUserData();
    if (rigidBody) {
        static_cast<RigidBody*>(rigidBody)->collisionEnd();
    }

    rigidBody = contact->GetFixtureB()->GetBody()->GetUserData();
    if (rigidBody) {
        static_cast<RigidBody*>(rigidBody)->collisionEnd();
    }
}


PhysicsGameManager::PhysicsGameManager() : GameManager() {
    b2Vec2 gravity(0.0f, 9.8f);
    gameInfo.world = new b2World(gravity);
    gameInfo.world->SetContactListener(&contactListener);
}

PhysicsGameManager::PhysicsGameManager(const char* t, int w, int h, Uint32 f) : GameManager(t, w, h, f) {
    b2Vec2 gravity(0.0f, 9.8f);
    gameInfo.world = new b2World(gravity);
    gameInfo.world->SetContactListener(&contactListener);
}

void PhysicsGameManager::loop() {
    while (!quit) {
        startTick = SDL_GetTicks();
        gameInfo.world->Step(1.0f/60.0f, 6, 2);
        events();
        update();
        SDL_RenderClear(gameInfo.renderer);
        draw();
        SDL_RenderPresent(gameInfo.renderer);
        capFps();
    }
    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(gameInfo.renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

PhysicsGameManager::~PhysicsGameManager() {
    delete gameInfo.world;
}
