#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Map.h"

enum  EntityType { PLAYER, PLATFORM, LIFE, ENEMY, BULLET };
enum AIType { WALKER };
enum AIState { IDLE, WALKING, UPDOWN, LEFTRIGHT, Flying, PATROLLING };

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

	int life;
	bool dead;
	bool shakeEffect;
	bool win;
	bool restart;
	float width;
	float height;

	Mix_Chunk *jump;
	Mix_Chunk *hit;
	float speed;

	GLuint textureID;

	Entity();

	bool CheckCollision(Entity other);

	void CheckCollisionsX(Entity *objects, int objectCount);
	void CheckCollisionsY(Entity *objects, int objectCount);

	void CheckCollisionsX(Map *map);
	void CheckCollisionsY(Map *map);

	void AI(Entity player);

	void Update(float deltaTime, Entity player, Entity *enemies, int enemyCount, Map *map);
	void Render(ShaderProgram *program);

	void Jump();

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;


	void ShootBullet(Entity player);
};



