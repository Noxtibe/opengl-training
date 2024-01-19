#include "Scene_028_ComputeShaderFX.h"
#include "../engine/MacroUtils.h"

#include <assert.h>
#include <fstream>
#include <sstream>
#include <iostream>

void Scene_028_ComputeShaderFX::load()
{
	srand( time( 0 ) );
	_time = rand();

	// Setup projection & view
	projection = Matrix4::createPerspectiveFOV(70.0f, _game->windowWidth, _game->windowHeight, 0.1f, 1000.0f);
	viewTransform.set_position(Vector3 { 0.0f, 0.0f, -150.0f });

	glEnable(GL_PROGRAM_POINT_SIZE);
	initializeParticles();
	initializeComputeShader();
	initializeRenderShader();
}

void Scene_028_ComputeShaderFX::clean()
{
}

void Scene_028_ComputeShaderFX::handleEvent(const InputState& state)
{
	if (state.keyboardState.isJustPressed(SDL_SCANCODE_L))
	{
		load();
	}

	if (state.keyboardState.isDown(SDL_SCANCODE_U))
	{
		simulation.timeScaling += 0.5f;
	}
	else if (state.keyboardState.isDown(SDL_SCANCODE_I))
	{
		simulation.timeScaling -= 0.5f;
	}
}

void Scene_028_ComputeShaderFX::update(float dt)
{
	deltaTime = dt * simulation.timeScaling;
	_time += deltaTime;
}

void Scene_028_ComputeShaderFX::draw()
{
	// Clear
	static const GLfloat bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, bgColor);

	// Compute Shader
	computeShader.use();
	computeShader.setFloat("dt", deltaTime);
	computeShader.setFloat("accelerationStrenght", simulation.accelerationStrenght);

	computeShader.setVector3f("minSpeedColor", simulation.minSpeedColor);
	computeShader.setVector3f("maxSpeedColor", simulation.maxSpeedColor);
	computeShader.setFloat("highSpeedMax", simulation.highSpeedMax);
	computeShader.setVector3f("implosionColor", simulation.implosionColor);
	computeShader.setFloat("implosionColorDistMax", simulation.implosionColorDistMax);

	computeShader.setInteger("sphereActivate", simulation.sphereActivate);
	computeShader.setFloat("sphereRadius", simulation.sphereRadius);
	computeShader.setFloat("sphereBounce", simulation.sphereBounce);
	glDispatchCompute(particlesNumber / groupSize, 1, 1);

	// Render Shader
	shader.use();
	shader.setMatrix4("projection", projection);
	shader.setMatrix4("view", viewTransform.get_matrix());
	shader.setFloat("pointSize", pointSize);
	glDrawArrays( GL_POINTS, 0, particlesNumber);
}

float Scene_028_ComputeShaderFX::getRandomInRange(float min, float max)
{
	return min + (float) (rand()) / ((float) (RAND_MAX / (max - min)));;
}

Vector3 Scene_028_ComputeShaderFX::getRandomPositionInSphere()
{
	Vector3 pos 
	{
		getRandomInRange(-1.0f, 1.0f),
		getRandomInRange(-1.0f, 1.0f),
		getRandomInRange(-1.0f, 1.0f)
	};
	pos.normalize();

	return pos;
}

void Scene_028_ComputeShaderFX::initializeParticles()
{
	std::vector<Vector4> positions(particlesNumber);
	std::vector<Vector4> velocities(particlesNumber);
	std::vector<Vector4> colors(particlesNumber);

	for (int i = 0; i < particlesNumber; i++)
	{
		positions[i] = Vector4 { getRandomPositionInSphere() * 50.0f, 1.0f };
		velocities[i] = Vector4 {};
		colors[i] = Vector4 { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	createSSBOs(positions, velocities, colors);
}

void Scene_028_ComputeShaderFX::initializeComputeShader()
{
	//  read source file
	std::ifstream shader_reader(SHADER_COMP(028_ComputeShaderFX));
    std::stringstream shader_stream;
	shader_stream << shader_reader.rdbuf();
	shader_reader.close();
	std::string source_code_str = shader_stream.str();
	const GLchar* source_code = source_code_str.c_str();

	computeShaderId = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShaderId, 1, &source_code, NULL);
	glCompileShader(computeShaderId);
	Shader::checkShaderErrors(computeShaderId, "compute");

	computeProgram = glCreateProgram();
	glAttachShader(computeProgram, computeShaderId);
	glLinkProgram(computeProgram);

	glDeleteShader(computeShaderId);

	computeShader.id = computeProgram;
	assert(computeShader.isValid());
}

void Scene_028_ComputeShaderFX::initializeRenderShader()
{
	shader = Assets::loadShader
	(
		SHADER_VERT(028_ComputeShaderFX),
		SHADER_FRAG(028_ComputeShaderFX),
		"",
		"",
		"",
		"028_ComputeShaderFX"
	);
	assert(shader.isValid());

	glEnableVertexAttribArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, ssboPositions);
	glVertexPointer(4, GL_FLOAT, 0, (void*) 0);
	glEnableVertexAttribArray( 1 );
	glBindBuffer(GL_ARRAY_BUFFER, ssboVelocities);
	glVertexPointer(4, GL_FLOAT, 0, (void*) 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, ssboColors);
	glVertexPointer(4, GL_FLOAT, 0, (void*) 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void Scene_028_ComputeShaderFX::createSSBOs
(
	const std::vector<Vector4>& positions,
	const std::vector<Vector4>& velocities,
	const std::vector<Vector4>& colors
)
{
	// Positions
	glGenBuffers(1, &ssboPositions);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboPositions);
	glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(Vector4), positions.data(), GL_STATIC_DRAW);

	// Velocities
	glGenBuffers(1, &ssboVelocities);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboVelocities );
	glBufferData(GL_SHADER_STORAGE_BUFFER, velocities.size() * sizeof(Vector4), velocities.data(), GL_STATIC_DRAW);

	// Colors
	glGenBuffers(1, &ssboColors);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboColors);
	glBufferData(GL_SHADER_STORAGE_BUFFER, colors.size() * sizeof(Vector4), colors.data(), GL_STATIC_DRAW);

	// Bind buffers bases
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboPositions);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssboVelocities);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssboColors);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// Unbind buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}