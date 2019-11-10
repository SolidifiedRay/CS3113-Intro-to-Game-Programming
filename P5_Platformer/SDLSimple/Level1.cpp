#include "Level1.h"
#define LEVEL1_WIDTH 30
#define LEVEL1_HEIGHT 8
unsigned int level1_data[] =
{
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.width = 1.0f;
	state.player.position = glm::vec3(4, 0, 0);
	state.player.acceleration = glm::vec3(0, -9.81f, 0);
	state.player.textureID = Util::LoadTexture("player.png");
	state.nextLevel = -1;

	GLuint devilTextureID = Util::LoadTexture("enemy.png");
	/*
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].textureID = devilTextureID;
	state.enemies[0].isStatic = false;
	state.enemies[0].position = glm::vec3(10, 0, 0);
	state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[0].aiState = PATROLLING;
	*/
	
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].textureID = devilTextureID;
	state.enemies[0].isStatic = false;
	state.enemies[0].position = glm::vec3(17, 2, 0);
	state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[0].aiState = IDLE;
	

}

void Level1::Update(float deltaTime) {
	if (state.player.dead == false and state.player.restart == true) {
		state.player.position = glm::vec3(4, 0, 0);
		state.player.restart = false;
	}
	state.player.Update(deltaTime, state.player, NULL, 0, state.enemies, ENEMY_COUNT, state.map);
	if (state.player.position.x > 22) {
		state.nextLevel = 2;
	}
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.enemies, ENEMY_COUNT, state.map);
	}

}

void Level1::Render(ShaderProgram *program) {
	state.map->Render(program);

	if (state.player.isActive == true) {
		state.player.Render(program);
	}

	for (int i = 0; i < ENEMY_COUNT; i++) {
		if (state.enemies[i].isActive == true) {
			state.enemies[i].Render(program);
		}
	}
}