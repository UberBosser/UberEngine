#include <iostream>
#include <UE/UE_core.h>
#include <UE/UE_utils.h>
#include <UE/UE_physics.h>
#include <UE/UE_audio.h>


class Floor : public StaticBody {
    public:
        Floor(GameInfo* g) : StaticBody(g) {
            loadImage(200, 400, "Assets/longBox.png");
            boxFixture();
        }
};


class Box : public DynamicBody {
    public:
        Box(GameInfo* g, GameObjects <Box>* b, SoundManager* s, int x, int y) : DynamicBody(g) {
            loadImage(x, y, "Assets/smallBox.png", 1.5);
            deathSound = s;

            boxes = b;
            boxFixture();
        }

        void update() {
            updatePhysics();
            if (rect.y > 512) {
                deathSound->play(); 
                boxes->remove(this);
                delete this;
            }
        }

    private:
        GameObjects <Box>* boxes;
        SoundManager* deathSound;
};


class Player : public DynamicBody {
    public:
        Player(GameInfo* g) : DynamicBody(g) {
            loadImage(0, 0, 32, 32, "Assets/circlePlayer.png", 2);
            jumpSound = new SoundManager("Assets/jump.wav");
            circleFixture();
            collisions = 0;
            canJump = true;
        }
        
        void collisionStart() {
            collisions++;
            canJump = true;
        }

        void collisionEnd() {
            collisions--;
        }

        void update() { 
            updatePhysics();
            const Uint8* keys = GET_KEYS();
            if (keys[KEY_A]) {
                setVelocity(-1, 0);
            }
            if (keys[KEY_D]) {
                setVelocity(1, 0);
            }
            if (keys[KEY_W] && canJump) {
                jumpSound->play();
                setVelocity(0, -5);
                canJump = false;
            }     
            if (keys[KEY_R]) {
                setPosition(0, 0);
                setVelocity(0, 0);
            }

            if (collisions > 0) {
                changeFrame(1);
            } else {
                changeFrame(0);
            }
        }

        ~Player() {
            delete jumpSound; 
        }

    private:
        SoundManager* jumpSound;
        int collisions;
        bool canJump;
};


class Game : public PhysicsGameManager {
    public:
        Game() : PhysicsGameManager("Physics Test", 512, 512, 0){
            camera = new GameCamera(getGameInfo(), 512, 512);
            music = new MusicManager("Assets/music.wav");
            // music->play();
            boxDeathSound = new SoundManager("Assets/knock.wav", 128);
            f = new Floor(getGameInfo());
            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                    boxes.add(new Box(getGameInfo(), &boxes, boxDeathSound, 300 + 16 * x, 200 + 20 * y) );
                }
            }
            p = new Player(getGameInfo());
            camera->setTarget(p);
        }

        void update() {
            f->update();
            boxes.update();
            p->update();
            camera->update();
        }

        void draw() {
            f->draw(camera);
            boxes.draw(camera);
            p->draw(camera);
        }

        ~Game() {
            delete music;
            delete boxDeathSound;
            delete f; 
            delete p;
            delete camera;
        }
    
    private:
        MusicManager* music;
        SoundManager* boxDeathSound;
        GameObjects <Box> boxes;
        Floor* f;
        Player* p;
        GameCamera* camera;
};

int main() {
    Game g;
    g.loop();
}
