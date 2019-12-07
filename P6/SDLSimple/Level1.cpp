#include "Level1.h"
#define LEVEL1_WIDTH 60
#define LEVEL1_HEIGHT 8
unsigned int level1_data[] =
{
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 3, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0, 0,
 3, 0, 0, 0, 3, 3, 0, 0, 3, 3, 3, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0, 0,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

void Level1::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.width = 1.0f;
	state.player.position = glm::vec3(3, -3.2, 0);
	state.player.acceleration = glm::vec3(0, 0, 0);
	state.player.textureID = Util::LoadTexture("player.png");

	state.bullet.entityType = BULLET;
	state.bullet.position = state.player.position;
	state.bullet.isStatic = false;
	state.bullet.isActive = false;
	state.bullet.textureID = Util::LoadTexture("bullet.png");

	GLuint alienTextureID = Util::LoadTexture("alien.png");
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].textureID = alienTextureID;
	state.enemies[0].isStatic = false;
	state.enemies[0].position = glm::vec3(15, -4, 0);
	state.enemies[0].origin_y = state.enemies[0].position.y;
	state.enemies[0].origin_x = state.enemies[0].position.x;
	state.enemies[0].aiState = UPDOWN;


	state.enemies[1].entityType = ENEMY;
	state.enemies[1].textureID = alienTextureID;
	state.enemies[1].isStatic = false;
	state.enemies[1].position = glm::vec3(22, -3.5, 0);
	state.enemies[1].origin_y = state.enemies[1].position.y;
	state.enemies[1].origin_x = state.enemies[1].position.x;
	state.enemies[1].aiState = UPDOWN;

	state.enemies[2].entityType = ENEMY;
	state.enemies[2].textureID = alienTextureID;
	state.enemies[2].isStatic = false;
	state.enemies[2].position = glm::vec3(33, -2.5, 0);
	state.enemies[2].origin_y = state.enemies[2].position.y;
	state.enemies[2].origin_x = state.enemies[2].position.x;
	state.enemies[2].aiState = UPDOWN;

	state.nextLevel = -1;
	
	state.player.velocity.x = 2;

}

void Level1::Update(float deltaTime) {

	state.player.velocity.x = 2;

	if (state.player.dead == false and state.player.restart == true) {
		state.player.position = glm::vec3(4, -3.5, 0);
		state.player.restart = false;
	}
	state.player.Update(deltaTime, &(state.player), state.enemies, ENEMY_COUNT, state.map);
	if (state.player.position.x > 55) {
		state.nextLevel = 2;
	}

	state.bullet.Update(deltaTime, &(state.player), state.enemies, ENEMY_COUNT, state.map);

	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, &(state.player), state.enemies, ENEMY_COUNT, state.map, state.enemies[i].origin_y, state.enemies[i].origin_x);
	}

}

void Level1::Render(ShaderProgram *program) {
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