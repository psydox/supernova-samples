#include "Supernova.h"
using namespace Supernova;

const float cubeSize = 1;
const float sphereSize = 1;
const float terrainSize = 20;


Vector2 lastTouchPos;

Scene scene;

Camera camera(&scene);
Shape terrain(&scene);
SkyBox sky(&scene);


Shape* cubes[4];
Shape* spheres[4];

Scene uiscene;
Text text(&uiscene);

void onKeyDown(int key, bool repeat, int mods);
void onTouchStart(int pointer, float x, float y);
void onTouchMove(int pointer, float x, float y);
void onMouseScroll(float xoffset, float yoffset, int mods);

void onContactAdded3D(Body3D bA, Body3D bB, Contact3D inContact){
    printf("onContactAdded3D %s %s\n", bA.getAttachedObject().getName().c_str(), bB.getAttachedObject().getName().c_str());
}

void onContactPersisted3D(Body3D bA, Body3D bB, Contact3D inContact){
    printf("onContactPersisted3D %s %s\n", bA.getAttachedObject().getName().c_str(), bB.getAttachedObject().getName().c_str());
}

void onContactRemoved3D(unsigned int bodyIDA, unsigned int bodyIDB, int subShape1, int subShape2){
    printf("onContactRemoved3D %u %u\n", bodyIDA, bodyIDB);
}

bool shouldCollide3D(Body3D bA, Body3D bB, Vector3 inBaseOffset, CollideShapeResult3D inCollisionResult){
    printf("shouldCollide3D %s %s\n", bA.getAttachedObject().getName().c_str(), bB.getAttachedObject().getName().c_str());

    return true;
}

void onBodyActivated3D(Body3D bA){
    printf("onBodyActivated3D %s\n", bA.getAttachedObject().getName().c_str());
}

void onBodyDeactivated3D(Body3D bA){
    printf("onBodyDeactivated3D %s\n", bA.getAttachedObject().getName().c_str());
}

void startPositions(){
    for (int i = 0; i < 4; i++){
        if (i < 2){
            cubes[i]->setPosition((4*i)-6, 11, -0.5);
        }else{
            cubes[i]->setPosition((4*i)-6, 11, 0.5);
        }
        cubes[i]->getBody3D().setLinearVelocity(Vector3(0, 0, 0));

        spheres[i]->setPosition((4*i)-5, 7, 0);
        spheres[i]->getBody3D().setLinearVelocity(Vector3(0, 0, 0));
    }
}


void init(){
    scene.getSystem<PhysicsSystem>()->onContactAdded3D = onContactAdded3D;
    scene.getSystem<PhysicsSystem>()->onContactPersisted3D = onContactPersisted3D;
    scene.getSystem<PhysicsSystem>()->onContactRemoved3D = onContactRemoved3D;
    scene.getSystem<PhysicsSystem>()->shouldCollide3D = shouldCollide3D;
    scene.getSystem<PhysicsSystem>()->onBodyActivated3D = onBodyActivated3D;
    scene.getSystem<PhysicsSystem>()->onBodyDeactivated3D = onBodyDeactivated3D;

    for (int i = 0; i < 4; i++){
        cubes[i] = new Shape(&scene);
        cubes[i]->createBox(cubeSize, cubeSize, cubeSize);
        cubes[i]->setTexture("block.png");
        cubes[i]->setName("cube"+std::to_string(i));
        cubes[i]->getBody3D().createBoxShape(cubeSize, cubeSize, cubeSize);

        spheres[i] = new Shape(&scene);
        spheres[i]->createSphere(sphereSize);
        spheres[i]->setTexture("block.png");
        spheres[i]->setName("sphere"+std::to_string(i));
        spheres[i]->getBody3D().createSphereShape(sphereSize);
    }
    startPositions();

    lastTouchPos = Vector2(0, 0);

    scene.setAmbientLight(0.5);
    scene.setCamera(camera.getEntity());

    text.setText("Press any key to reset");
    text.setAnchorPreset(AnchorPreset::CENTER_TOP);
    text.setName("text");

    camera.setPosition(0, 7, -20);
    camera.setView(0, 2, 0);

    terrain.createPlane(terrainSize, terrainSize);
    terrain.setTexture("ground.png");
    terrain.setName("terrain");
    Body3D bodyterrain = terrain.getBody3D();
    bodyterrain.setType(BodyType::STATIC);
    bodyterrain.createMeshShape();

    Light* sun = new Light(&scene);
    sun->setType(LightType::DIRECTIONAL);
    sun->setDirection(0,-0.6, 0.4);
    sun->setIntensity(4);
    sun->setShadows(true);

    sky.setTextureFront("ely_hills/hills_lf.tga");
    sky.setTextureBack("ely_hills/hills_rt.tga");
    sky.setTextureLeft("ely_hills/hills_bk.tga");
    sky.setTextureRight("ely_hills/hills_ft.tga");
    sky.setTextureUp("ely_hills/hills_up.tga");
    sky.setTextureDown("ely_hills/hills_dn.tga");

    Engine::setScalingMode(Scaling::FITWIDTH);
    Engine::setScene(&scene);
    Engine::addSceneLayer(&uiscene);
    Engine::setCallTouchInMouseEvent(true);

    Engine::onKeyDown = onKeyDown;
    Engine::onTouchStart = onTouchStart;
    Engine::onTouchMove = onTouchMove;
    Engine::onMouseScroll = onMouseScroll;
}

void onKeyDown(int key, bool repeat, int mods){
    startPositions();
}

void onTouchMove(int pointer, float x, float y){
    float difX = lastTouchPos.x - x;
    float difY = lastTouchPos.y - y;
    lastTouchPos = Vector2(x, y);

    camera.rotatePosition(0.1 * difX);
    camera.elevatePosition(-0.1 * difY);
}

void onTouchStart(int pointer, float x, float y){
    lastTouchPos = Vector2(x, y);
}

void onMouseScroll(float xoffset, float yoffset, int mods){
    camera.moveForward(0.5 * yoffset);
    camera.setView(0, 2, 0);
}