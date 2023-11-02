#ifndef Scene_027_TesselationCube_H
#define Scene_027_TesselationCube_H

#include "../engine/Scene.h"
#include "../engine/Assets.h"

#include "CubeObject.h"

class CubeMesh;

class Scene_027_TesselationCube : public Scene 
{
public:

    Scene_027_TesselationCube();
    ~Scene_027_TesselationCube();
    void load();
    void clean();
    void pause();
    void resume();
    void handleEvent(const InputState &);
    void update(float dt);
    void draw();
    void setGame(Game *);

private:

    Game *game;
    CubeMesh* cubeMesh;

    Matrix4 projection;
    Shader shader;

    vector<CubeObject> cubes;
    float newXPosition { 0 };
};


#endif //Scene_027_TesselationCube_H
