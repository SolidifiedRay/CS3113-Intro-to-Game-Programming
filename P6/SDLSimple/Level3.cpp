#include "Level3.h"
#define LEVEL3_WIDTH 30
#define LEVEL3_HEIGHT 8
unsigned int level3_data[] =
{
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 1, 1, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level3::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.width = 1.0f;
	state.player.position = glm::vec3(2, 0, 0);
	state.player.acceleration = glm::vec3(0, -9.81f, 0);
	state.player.textureID = Util::LoadTexture("player.png");
	state.nextLevel = -3;

	GLuint devilTextureID = Util::LoadTexture("enemy.png");
	GLuint flyingDevilTextureID = Util::LoadTexture("flying_enemy.png");
	state.enemies[0].entityType = ENEMY;
	state.enemies[0].textureID = flyingDevilTextureID;
	state.enemies[0].isStatic = false;
	state.enemies[0].position = glm::vec3(6.5, -2, 0);
	state.enemies[0].aiState = UPDOWN;

	state.enemies[1].entityType = ENEMY;
	state.enemies[1].textureID = devilTextureID;
	state.enemies[1].isStatic = false;
	state.enemies[1].position = glm::vec3(10, -2, 0);
	state.enemies[1].aiState = UPDOWN;

	state.enemies[2].entityType = ENEMY;
	state.enemies[2].textureID = flyingDevilTextureID;
	state.enemies[2].isStatic = false;
	state.enemies[2].position = glm::vec3(12.5, -2, 0);
	state.enemies[2].aiState = UPDOWN;

	state.enemies[3].entityType = ENEMY;
	state.enemies[3].textureID = flyingDevilTextureID;
	state.enemies[3].isStatic = false;
	state.enemies[3].position = glm::vec3(15.5, -2, 0);
	state.enemies[3].aiState = UPDOWN;

	state.bullet.entityType = BULLET;
	state.bullet.position = state.player.position;
	state.bullet.isStatic = false;
	state.bullet.isActive = false;
	state.bullet.textureID = Util::LoadTexture("bullet.png");
}
void Level3::Update(float deltaTime) {
	if (state.player.dead == false and state.player.restart == true) {
		state.player.position = glm::vec3(4, 0, 0);
		state.player.restart = false;
	}
	state.player.Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	if (state.player.position.x > 23) {
		state.player.win = true;
	}

	state.bullet.Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);

	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, state.player, state.enemies, ENEMY_COUNT, state.map);
	}
}
void Level3::Render(ShaderProgram* program) {
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