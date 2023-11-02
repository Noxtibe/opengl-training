#ifndef Scene_026_Custom_H
#define Scene_026_Custom_H

#include "../engine/Scene.h"
#include "../engine/Assets.h"


class Scene_026_Custom : public Scene 
{
public:

    Scene_026_Custom();
    ~Scene_026_Custom();
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
    GLuint vao;
    GLuint vbo;
    GLuint buffer;
    Matrix4 transform[24];
    Matrix4 projection;

    Shader shader;
};


#endif //Scene_026_Custom_H
