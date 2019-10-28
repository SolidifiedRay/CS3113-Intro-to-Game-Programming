#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum  EntityType { PLAYER, PLATFORM, COIN, ENEMY, TEXT };
enum AIType {WALKER};
enum AIState { IDLE, WALKING, Flying, PATROLLING};

class Entity {
public:

	EntityType entityType;
	AIType aitype;
	AIState aiState;

	bool isStatic;
	bool isActive;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	float width;
	float height;

	float speed;

	GLuint textureID;
	std::string text;

	Entity();

	bool CheckCollision(Entity other);

	void CheckCollisionsX(Entity *objects, int objectCount);
	void CheckCollisionsY(Entity *objects, int objectCount);

	void AI(Entity player);
	void Update(float deltaTime, Entity player, Entity *objects, int objectCount, Entity *enemies, int enemyCount);
	void Render(ShaderProgram *program);

	void Jump();

	void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

};



