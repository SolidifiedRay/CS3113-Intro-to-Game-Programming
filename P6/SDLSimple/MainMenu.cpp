#include "MainMenu.h"
#define MAINMENU_WIDTH 30
#define MAINMENU_HEIGHT 8
unsigned int mainmenu_data[] =
{
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 3, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 3, 3, 0, 0, 3, 3, 3, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void MainMenu::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(MAINMENU_WIDTH, MAINMENU_HEIGHT, mainmenu_data, mapTextureID, 1.0f, 4, 1);
	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.width = 1.0f;
	state.player.position = glm::vec3(3, -3.2, 0);
	state.player.acceleration = glm::vec3(0, -9.8f, 0);
	state.player.textureID = Util::LoadTexture("player.png");
	state.nextLevel = -4;
}
void MainMenu::Update(float deltaTime) {}
void MainMenu::Render(ShaderProgram* program) {
	state.map->Render(program);

	if (state.player.isActive == true) {
		state.player.Render(program);
	}
}