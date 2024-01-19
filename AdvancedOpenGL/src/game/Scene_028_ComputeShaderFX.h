#ifndef Scene_028_ComputeShaderFX_H
#define Scene_028_ComputeShaderFX_H

#include "../engine/Scene.h"
#include "../engine/Assets.h"
#include "../engine/MeshObject.h"
#include "Transform.hpp"

class Scene_028_ComputeShaderFX : public Scene 
{
public:

	~Scene_028_ComputeShaderFX() { clean(); }
	void setGame(Game* game) { _game = game; }
	void load();
	void clean();
	void pause() {}
	void resume() {}
	void handleEvent( const InputState& state );
	void update( float dt );
	void draw();

private:

	float getRandomInRange(float min, float max);
	Vector3 getRandomPositionInSphere();

	void initializeParticles();
	void initializeComputeShader();
	void initializeRenderShader();

	void createSSBOs
    ( 
		const std::vector<Vector4>& positions,
		const std::vector<Vector4>& velocities,
		const std::vector<Vector4>& colors
	);

private:

	const int particlesNumber = 50000;
	const int groupSize = 100;
	const float pointSize = 10.0f;

private:

	Game* _game { nullptr };
	Shader shader;
	Shader computeShader;

	GLuint computeShaderId;
	GLuint computeProgram;

	GLuint vao;
	GLuint ssboPositions;
	GLuint ssboVelocities;
	GLuint ssboColors;

	float _time = 0.0f;
	float deltaTime = 0.0f;

	struct 
    {
		float timeScaling = 3.0f;
		float accelerationStrenght = 30.0f;
		Vector3 minSpeedColor { 1.0f, 0.5f, 0.0f };
		Vector3 maxSpeedColor { 0.8f, 0.0f, 0.0f };
		float highSpeedMax = 20.0f;
		Vector3 implosionColor { 1.0f, 1.0f, 1.0f };
		float implosionColorDistMax = 35.0f;
		bool sphereActivate= true;
		float sphereRadius = 60.0f;
		float sphereBounce = 0.3f;
	} simulation;

	Matrix4 projection;
	Transform viewTransform;
};

#endif //Scene_028_ComputeShaderFX
