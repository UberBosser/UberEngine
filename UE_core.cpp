#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "UE_core.h"


bool collideRect(SDL_Rect* a, SDL_Rect* b) {
    if (a->y + a->h <= b->y) {
            return false;
        }
        if (a->y >= b->y + b->h) {
            return false; 
        }
        if (a->x + a->w <= b->x) {
            return false;
        }
        if (a->x >= b->x + b->w) {
            return false; 
        }
        return true;
}


SpriteObject::SpriteObject(GameInfo* g) {
    gameInfo = g;
    flip = SDL_FLIP_NONE;
    angle = 0;
}

void SpriteObject::loadImage(int x, int y, const char* i) {
    surface = IMG_Load(i);
    if (!surface) {
        printf("Couldn't load image: %s\n", IMG_GetError());
    }
    rect = surface->clip_rect;
    dRect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2; 
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
    SDL_FreeSurface(surface);
}

void SpriteObject::loadImage(int x, int y, int w, int h, const char* i) {
    surface = IMG_Load(i);
    if (!surface) {
        printf("Couldn't load image: %s\n", IMG_GetError());
    }
    rect = surface->clip_rect;
    for (int y = 0; y < rect.h/h; y++) {
        for (int x = 0; x < rect.w/w; x++) {
            SDL_Rect r = {x * w, y * h, w, h};
            frames.push_back(r);
        }
    }
    dRect = frames[0];
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;
    pivot.x = dRect.w/2;
    pivot.y = dRect.h/2;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
    SDL_FreeSurface(surface);
}

void SpriteObject::loadImage(int x, int y, SDL_Surface* i) {
    surface = i;
    if (!surface) {
        printf("Couldn't load image: %s\n", IMG_GetError());
    }
    rect = surface->clip_rect;
    dRect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
}

void SpriteObject::loadImage(int x, int y, int w, int h, SDL_Surface* i) {
    surface = i;
    if (!surface) {
        printf("Couldn't load image: %s\n", IMG_GetError());
    }
    rect = surface->clip_rect;
    for (int y = 0; y < rect.h/h; y++) {
        for (int x = 0; x < rect.w/w; x++) {
            SDL_Rect r = {x * w, y * h, w, h};
            frames.push_back(r);
        }
    }
    dRect = frames[0];
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;
    pivot.x = dRect.w/2;
    pivot.y = dRect.h/2;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    texture = SDL_CreateTextureFromSurface(gameInfo->renderer, surface);
}

SDL_Rect* SpriteObject::getRect() {
    return &rect;
}

int SpriteObject::getPosX() {
    return rect.x;
}

int SpriteObject::getPosY() {
    return rect.y;
}

void SpriteObject::changeFrame(int i) { 
    dRect = frames[i];
}

void SpriteObject::update() {}

void SpriteObject::draw() {
    drawTexture();
}

void SpriteObject::drawTexture() {
    if (collideRect(&rect, &gameInfo->screenRect)) {
        SDL_RenderCopyEx(gameInfo->renderer, texture, &dRect, &rect, angle, &pivot, flip);
    }
}

void SpriteObject::draw(SpriteObject* c) {
    drawTexture(c);
}

void SpriteObject::drawTexture(SpriteObject* c) {
    // Set offset
    if (collideRect(&rect, c->getRect())) {
        offsetRect.x = rect.x - c->getPosX();
        offsetRect.y = rect.y - c->getPosY();
        offsetRect.w = rect.w;
        offsetRect.h = rect.h;
        SDL_RenderCopyEx(gameInfo->renderer, texture, &dRect, &offsetRect, angle, &pivot, flip);
    }
}

SpriteObject::~SpriteObject() {
    SDL_DestroyTexture(texture);
}


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
    boxShape.SetAsBox((dRect.w * 0.01f)/2.0f - 0.01f, (dRect.h * 0.01f)/2.0f - 0.01f);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;

    body->CreateFixture(&boxFixtureDef);

    setPosition(rect.x * 0.01f, rect.y * 0.01f);
}

void StaticBody::circleFixture() {
    b2CircleShape circleShape;
    circleShape.m_radius = (dRect.w/2 + dRect.h/2)/2 * 0.01;

    b2FixtureDef circleFixtureDef;
    circleFixtureDef.shape = &circleShape;

    body->CreateFixture(&circleFixtureDef);
    
    setPosition(rect.x * 0.01f, rect.y * 0.01f);
}

void StaticBody::setPosition(float x, float y) {
    body->SetTransform(b2Vec2(x + (dRect.w * 0.01f)/2.0f, y + (dRect.h * 0.01f)/2.0f), body->GetAngle());
}


DynamicBody::DynamicBody(GameInfo* g) : RigidBody(g){
    body->SetType(b2_dynamicBody);    
}

void DynamicBody::boxFixture() {
    b2PolygonShape boxShape;
    boxShape.SetAsBox((dRect.w * 0.01f)/2.0f - 0.01f, (dRect.h * 0.01f)/2.0f - 0.01f);
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    boxFixtureDef.restitution = 0.3;

    body->CreateFixture(&boxFixtureDef);
    
    setPosition(rect.x * 0.01f, rect.y * 0.01f); 
}

void DynamicBody::circleFixture() {
    b2CircleShape circleShape;
    circleShape.m_radius = (dRect.w/2 + dRect.h/2)/2 * 0.01;

    b2FixtureDef circleFixtureDef;
    circleFixtureDef.shape = &circleShape;
    circleFixtureDef.density = 1;
    circleFixtureDef.restitution = 0.3;

    body->CreateFixture(&circleFixtureDef);
    
    setPosition(rect.x * 0.01f, rect.y * 0.01f);
}

void DynamicBody::setPosition(float x, float y) {
    body->SetTransform(b2Vec2(x + (dRect.w * 0.01f)/2.0f, y + (dRect.h * 0.01f)/2.0f), body->GetAngle());
}

void DynamicBody::setVelocity(int x, int y) {
    // Fix int to box2d meters.
    body->SetLinearVelocity(b2Vec2(x, y));
}

void DynamicBody::update() {
    updatePhysics();
}

void DynamicBody::updatePhysics() {
    b2Vec2 pos = body->GetPosition();
    rect.x = (pos.x - (dRect.w * 0.01f)/2.0f) * 100;
    rect.y = (pos.y - (dRect.h * 0.01f)/2.0f) * 100;
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


SoundManager::SoundManager(const char *d) {
    sound = Mix_LoadWAV(d);
    if (!sound) {
        printf("Couldn't load sound: %s\n", Mix_GetError());
    }
    Mix_VolumeChunk(sound, 64);
}

SoundManager::SoundManager(const char* d, const int v) {
    sound = Mix_LoadWAV(d);
    if (!sound) {
        printf("Couldn't load sound: %s\n", Mix_GetError());
    }
    Mix_VolumeChunk(sound, v);
}

void SoundManager::play() {
    Mix_PlayChannel(-1, sound, 0);
}

void SoundManager::play(int l) {
    Mix_PlayChannel(-1, sound, l);
}

SoundManager::~SoundManager() {
    Mix_FreeChunk(sound);
}


MusicManager::MusicManager(const char* d) {
    music = Mix_LoadMUS(d);
    if (!music) {
        printf("Couldn't load music: %s\n", Mix_GetError());
    }
}

void MusicManager::play() {
    Mix_PlayMusic(music, -1);
}

void MusicManager::play(int l) {
    Mix_PlayMusic(music, l);
}

MusicManager::~MusicManager() {
    Mix_FreeMusic(music);
}


GameManager::GameManager() {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();
    window = SDL_CreateWindow("UberEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1366, 768, SDL_WINDOW_FULLSCREEN_DESKTOP);
    gameInfo.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1536);
    gameInfo.screenRect.w = 1366;
    gameInfo.screenRect.h = 768;
    SDL_SetWindowIcon(window, IMG_Load("Assets/icon.png"));

    b2Vec2 gravity(0.0f, 9.8f);
    gameInfo.world = new b2World(gravity);

    gameInfo.world->SetContactListener(&contactListener);
}

GameManager::GameManager(const char* t, int w, int h, Uint32 f) {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG); 
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();
    window = SDL_CreateWindow(t, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, f);
    gameInfo.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1536);
    gameInfo.screenRect.w = w;
    gameInfo.screenRect.h = h;
    SDL_SetWindowIcon(window, IMG_Load("Assets/icon.png"));

    b2Vec2 gravity(0.0f, 9.8f);
    gameInfo.world = new b2World(gravity);
    
    gameInfo.world->SetContactListener(&contactListener);
}

GameInfo* GameManager::getGameInfo() {
    return &gameInfo;
}

int GameManager::getScreenWidth() {
    return gameInfo.screenRect.w;
}

int GameManager::getScreenHeight() {
    return gameInfo.screenRect.h;
}

void GameManager::setQuit(bool q) {
    quit = q;
}

void GameManager::events() {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true; 
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
    }
}

void GameManager::update() {}

void GameManager::draw() {}

void GameManager::capFps() {
    if ((1000/60) > SDL_GetTicks() - startTick) {
        SDL_Delay(1000/60 - (SDL_GetTicks() - startTick));
    }
}

void GameManager::loop() {
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
    SDL_Quit();
}

GameManager::~GameManager() {
    delete gameInfo.world;
}
