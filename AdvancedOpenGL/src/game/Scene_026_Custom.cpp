#include "Scene_026_Custom.h"
#include "../engine/Timer.h"
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

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    projection = Matrix4::createPerspectiveFOV(70.0f, game->windowWidth, game->windowHeight, 0.1f, 1000.0f);

    // Les coordonnées du point
    static const GLfloat pointPosition[] = {0.0f, 0.0f, 0.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(pointPosition), pointPosition, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    shader = Assets::getShader(SHADER_ID(SHADER_NAME));
}

void Scene_026_Custom::clean()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
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
    // Aucune mise à jour nécessaire pour un point statique
}

void Scene_026_Custom::draw()
{
    static const GLfloat bgColor[] = {0.0f, 0.0f, 0.2f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, bgColor);

    shader.use();

    // Dessinez le point
    glDrawArrays(GL_POINTS, 0, 1);
}
