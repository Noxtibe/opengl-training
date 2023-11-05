#include "Scene_026_Custom.h"
#include "../engine/MacroUtils.h"
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>

Scene_026_Custom::Scene_026_Custom() 
{
}

Scene_026_Custom::~Scene_026_Custom() 
{
    clean();
}

void Scene_026_Custom::setGame(Game *_game) 
{
    game = _game;
}

void Scene_026_Custom::load() 
{
    std::srand((int) std::time(nullptr));
    Assets::loadShader(SHADER_VERT(SHADER_NAME), SHADER_FRAG(SHADER_NAME), "", "", "", SHADER_ID(SHADER_NAME));

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    shader = Assets::getShader(SHADER_ID(SHADER_NAME));
}

void Scene_026_Custom::clean() 
{
    glDeleteVertexArrays(1, &vao);
}

void Scene_026_Custom::pause() 
{
}

void Scene_026_Custom::resume() 
{
}

void Scene_026_Custom::handleEvent(const InputState &inputState) 
{
}

void Scene_026_Custom::update(float dt) 
{
    timeSinceStart = (float)SDL_GetTicks() / 1000.0f;
}

void Scene_026_Custom::draw() 
{

    static const GLfloat bgColor[] = {0.0f, 0.0f, 0.2f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, bgColor);

    GLfloat offset[] = { (float)sin(timeSinceStart) * 0.5f + 0.5f, (float)cos(timeSinceStart) *0.5f + 0.5f, 0.0f, 1.0f};
    glVertexAttrib4fv(0, offset);
    shader.use();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}