#include "Scene_027_TesselationCube.h"
#include "../engine/Timer.h"
#include "../engine/MacroUtils.h"
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>
#include <cmath>
#include "CubeMesh.h"

Scene_027_TesselationCube::Scene_027_TesselationCube()
{

}

Scene_027_TesselationCube::~Scene_027_TesselationCube() 
{
	clean();
}

void Scene_027_TesselationCube::setGame(Game* _game) 
{
	game = _game;
}

GLuint vao;

void Scene_027_TesselationCube::load() 
{
	std::srand((int)std::time(nullptr));
	Assets::loadShader(SHADER_VERT(SHADER_NAME), SHADER_FRAG(SHADER_NAME), SHADER_TECS(SHADER_NAME), SHADER_TESE(SHADER_NAME), "", SHADER_ID(SHADER_NAME));
	projection = Matrix4::createPerspectiveFOV(50.1f, game->windowWidth, game->windowHeight, 0.1f, 1000.0f);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	cubeMesh = new CubeMesh();
	cubeMesh->load();

	shader = Assets::getShader(SHADER_ID(SHADER_NAME));

	cubes.emplace_back(0.0f, 0.0f, cubeMesh);
}

void Scene_027_TesselationCube::clean() 
{
	cubeMesh->clean();
	delete cubeMesh;
}

void Scene_027_TesselationCube::pause() 
{

}

void Scene_027_TesselationCube::resume() 
{

}

void Scene_027_TesselationCube::handleEvent(const InputState& inputState) 
{

}

void Scene_027_TesselationCube::update(float dt) 
{
	for (auto& cube : cubes) 
    {
		cube.update();
	}
	float formerXPosition = cubes[0].getX();
	cubes[0].setPosition(newXPosition, cubes[0].getX());
	newXPosition = std::sinf(gameTime) / 10.0f;
	gameTime += dt * -0.5f;
}

void Scene_027_TesselationCube::draw() 
{
	static const GLfloat bgColor[] = { 0.0f, 0.0f, 0.2f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, bgColor);
	shader.use();
	shader.setMatrix4("proj_matrix", projection);
	shader.setInteger("tessLevel", 1 + (int)((0.5f + (std::sinf(gameTime) / 2.0f)) * 30));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (auto& cube : cubes) 
	{
		cube.drawTesselation(shader);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}