#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Entity.h"
#include "Effects.h"
#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "MainMenu.h"

Scene *currentScene;
Scene *sceneList[4];

Effects *effects;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::vec3 lifeVec, gameOverVec;

GLuint fontTextureID;

int nextLevelLife;
Mix_Music *bgm;

void SwitchToScene(Scene *scene) {
	currentScene = scene;
	currentScene->Initialize();
}


void Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context); 
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	bgm = Mix_LoadMUS("bgm.wav");

	Mix_PlayMusic(bgm, -1);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);


	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	//program.Load("shaders/vertex_textured.glsl","shaders/effects_textured.glsl");
	//program.Load("shaders/vertex_lit.glsl","shaders/fragment_lit.glsl");



	fontTextureID = Util::LoadTexture("font1.png");

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	glUseProgram(program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	sceneList[0] = new MainMenu();
	sceneList[1] = new Level1();
	sceneList[2] = new Level2();
	sceneList[3] = new Level3();
	SwitchToScene(sceneList[0]);
	nextLevelLife = 3;

	int total_life = currentScene->state.player.life;

	effects = new Effects(projectionMatrix, viewMatrix);
	effects->Start(FADEIN, 0.5f);
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				currentScene->state.player.Jump();
				break;
			case SDLK_RETURN:
				if (currentScene->state.nextLevel == -4) {
					SwitchToScene(sceneList[1]);
				}
				break;
			case SDLK_k:
				effects->Start(SHAKE, 2.0f);
				break;
			}
		}
	}

	currentScene->state.player.velocity.x = 0;

	// Check for pressed/held keys below
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_A])
	{
		currentScene->state.player.velocity.x = -3.0f;
	}
	else if (keys[SDL_SCANCODE_D])
	{
		currentScene->state.player.velocity.x = 3.0f;
	}
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {

		//state.player.Update(FIXED_TIMESTEP, state.enemies, ENEMY_COUNT, state.map);
		currentScene->Update(FIXED_TIMESTEP);
		effects->Update(FIXED_TIMESTEP);

		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;

	//state.player.Update(FIXED_TIMESTEP, state.enemies, ENEMY_COUNT, state.map);

	viewMatrix = glm::mat4(1.0f);
	lifeVec = glm::vec3(1, -0.5, 0);
	gameOverVec = glm::vec3(3, -2, 0);
	
	if (currentScene->state.player.position.x > 5) {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player.position.x, 3.75, 0));
		lifeVec = glm::vec3(currentScene->state.player.position.x - 4, -0.5, 0);
		gameOverVec = glm::vec3(currentScene->state.player.position.x - 1.5, -2, 0);
	}
	else {
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
	}

	viewMatrix = glm::translate(viewMatrix, effects->viewTranslate);

	nextLevelLife = currentScene->state.player.life;

	if (currentScene->state.player.shakeEffect) {
		effects->Start(SHAKE, 2.0f);
		currentScene->state.player.shakeEffect = false;
	}

	if (currentScene->state.player.restart && !currentScene->state.player.dead) {
		effects->Start(FADEIN, 0.5f);
	}
}


void Render() {
	glUseProgram(program.programID);
	program.SetViewMatrix(viewMatrix);
	//program.SetLightPosition(currentScene->state.player.position);
	//program.SetLightPosition2(glm::vec3(15, -5, 0));
	glClear(GL_COLOR_BUFFER_BIT);


	if (currentScene->state.player.life == 3 && currentScene->state.nextLevel != -4) {
		Util::DrawText(&program, fontTextureID, "Lives: 3", 0.8f, -0.5f, lifeVec);
	}

	if (currentScene->state.player.life == 2) {
		Util::DrawText(&program, fontTextureID, "Lives: 2", 0.8f, -0.5f, lifeVec);
	}

	if (currentScene->state.player.life == 1) {
		Util::DrawText(&program, fontTextureID, "Lives: 1", 0.8f, -0.5f, lifeVec);
	}

	if (currentScene->state.player.life <= 0 ) {
		Util::DrawText(&program, fontTextureID, "You Lose", 1.0f, -0.5f, gameOverVec);
	}

	if (currentScene->state.player.win) {
		Util::DrawText(&program, fontTextureID, "You Win!", 1.0f, -0.5f, gameOverVec);
	}

	if (currentScene->state.nextLevel == -4) {
		Util::DrawText(&program, fontTextureID, "PLATFORMER", 1.0f, -0.5f, glm::vec3(2.5, -2, 0));
		Util::DrawText(&program, fontTextureID, "Press Enter to Play", 1.0f, -0.5f, glm::vec3(0.5, -4, 0));
	}

	if (currentScene->state.nextLevel == -1 && currentScene->state.player.life > 0) {
		Util::DrawText(&program, fontTextureID, "LEVEL1", 1.0f, -0.5f, glm::vec3(1, -2, 0));
		Util::DrawText(&program, fontTextureID, "Press 'A' and 'D' to move", 0.8f, -0.5f, glm::vec3(1, -3, 0));
		Util::DrawText(&program, fontTextureID, "Press 'Space' to jump", 0.8f, -0.5f, glm::vec3(1, -4, 0));
		Util::DrawText(&program, fontTextureID, "Try to step on the enemy's head", 0.8f, -0.5f, glm::vec3(10, -3, 0));
		Util::DrawText(&program, fontTextureID, "Next Level ->", 0.8f, -0.5f, glm::vec3(22, -3, 0));
	}
	if (currentScene->state.nextLevel == -2 && currentScene->state.player.life > 0) {
		Util::DrawText(&program, fontTextureID, "LEVEL2", 1.0f, -0.5f, glm::vec3(1, -2, 0));
		Util::DrawText(&program, fontTextureID, "Next Level ->", 0.8f, -0.5f, glm::vec3(22, -3, 0));
	}
	if (currentScene->state.nextLevel == -3 && currentScene->state.player.life > 0) {
		Util::DrawText(&program, fontTextureID, "LEVEL3", 1.0f, -0.5f, glm::vec3(1, -2, 0));
		Util::DrawText(&program, fontTextureID, "GOAL ->", 0.8f, -0.4f, glm::vec3(22, -3, 0));
	}

	currentScene->Render(&program);


	effects->Render();
	SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
	Mix_FreeMusic(bgm);
	Mix_FreeChunk(currentScene->state.player.hit);
	Mix_FreeChunk(currentScene->state.player.jump);
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		if (currentScene->state.nextLevel >= 0) {
			SwitchToScene(sceneList[currentScene->state.nextLevel]);
			effects->Start(FADEIN, 0.5f);
			currentScene->state.player.life = nextLevelLife;
		}
		if ((!currentScene->state.player.dead) && (!currentScene->state.player.win)) {
			ProcessInput();
		}
		else{
			currentScene->state.player.velocity.x = 0;
		}
		Update();
		Render();
	}

	Shutdown();
	return 0;
}
