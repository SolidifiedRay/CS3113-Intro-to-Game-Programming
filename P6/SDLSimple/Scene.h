#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"
#include "Map.h"

#define ENEMY_COUNT 4

struct GameState {
	Entity player;
	Entity bullet;
	Entity enemies[ENEMY_COUNT];
	Entity boss;
	Map *map;
	int nextLevel;
};
class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram *program) = 0;
};