#include "MainMenu.h"
#define MAINMENU_WIDTH 14
#define MAINMENU_HEIGHT 8
unsigned int mainmenu_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void MainMenu::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(MAINMENU_WIDTH, MAINMENU_HEIGHT, mainmenu_data, mapTextureID, 1.0f, 4, 1);
	state.nextLevel = -4;
}
void MainMenu::Update(float deltaTime) {}
void MainMenu::Render(ShaderProgram* program) {
	state.map->Render(program);
}