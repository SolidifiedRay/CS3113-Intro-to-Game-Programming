#include "Level2.h"
#define LEVEL2_WIDTH 30
#define LEVEL2_HEIGHT 8
unsigned int level2_data[] =
{
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level2::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.width = 1.0f;
	state.player.position = glm::vec3(4, 0, 0);
	state.player.acceleration = glm::vec3(0, -9.81f, 0);
	state.player.textureID = Util::LoadTexture("player.png");
	state.nextLevel = -2;

	GLuint devilTextureID = Util::LoadTexture("enemy.png");
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].textureID = devilTextureID;
	state.enemies[0].isStatic = false;
	state.enemies[0].position = glm::vec3(9, 0, 0);
	state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[0].aiState = UPDOWN;

	state.enemies[1].entityType = ENEMY;
	state.enemies[1].textureID = devilTextureID;
	state.enemies[1].isStatic = false;
	state.enemies[1].position = glm::vec3(12, 2, 0);
	state.enemies[1].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[1].aiState = LEFTRIGHT;

	state.enemies[2].entityType = ENEMY;
	state.enemies[2].textureID = devilTextureID;
	state.enemies[2].isStatic = false;
	state.enemies[2].position = glm::vec3(15, 2, 0);
	state.enemies[2].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[2].aiState = IDLE;

	state.enemies[3].entityType = ENEMY;
	state.enemies[3].textureID = devilTextureID;
	state.enemies[3].isStatic = false;
	state.enemies[3].position = glm::vec3(14, -3, 0);
	state.enemies[3].acceleration = glm::vec3(0, -9.81f, 0);
	state.enemies[3].aiState = IDLE;

	state.bullet.entityType = BULLET;
	state.bullet.position = state.player.position;
	state.bullet.isStatic = false;
	state.bullet.isActive = false;
	state.bullet.textureID = Util::LoadTexture("bullet.png");
}

void Level2::Update(float deltaTime) {
	if (state.player.dead == false and state.player.restart == true) {
		state.player.position = glm::vec3(4, 0, 0);
		state.player.restart = false;
	}
	state.player.Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	if (state.player.position.x > 22) {
		state.nextLevel = 3;
	}

	state.bullet.Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);

	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	}
}
void Level2::Render(ShaderProgram *program) {
	state.map->Render(program);

	if (state.player.isActive == true) {
		state.player.Render(program);
	}

	if (state.bullet.isActive == true) {
		state.bullet.Render(program);
	}

	for (int i = 0; i < ENEMY_COUNT; i++) {
		if (state.enemies[i].isActive == true) {
			state.enemies[i].Render(program);
		}
	}
}