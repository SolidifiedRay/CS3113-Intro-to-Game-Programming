#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum EntityType { PLAYER, PLATFORM, COIN};

class Entity {
public:
	EntityType entityType;
	bool isStatic;
	bool isActive;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	float speed;
	float width;
	float height;

	GLuint textureID;

	Entity();

	bool checkCollision(Entity other);

	void CheckCollisionsY(Entity *objects, int objectCount);
	void CheckCollisionsX(Entity *objects, int objectCount);

	void Update(float deltaTime, Entity* objects, int objectCoun);
	void Render(ShaderProgram* program);
	void Jump();
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
};


