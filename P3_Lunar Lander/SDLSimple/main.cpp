#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

#define PLATFORM_COUNT 35


struct GameState {
	Entity player;
	Entity platforms[PLATFORM_COUNT];
	Entity success_text;
	Entity failed_text;
};

GameState state;


GLuint LoadTexture(const char* filePath) {
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(image);
	return textureID;
}

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Lunar Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	state.player.entityType = PLAYER;
	state.player.isStatic = false;
	state.player.collideGoal = false;
	state.player.gameEnd = false;
	state.player.width = 0.7f;
	state.player.position = glm::vec3(-2, 3, 0);
	state.player.acceleration = glm::vec3(0.0f, -0.70f, 0.0f);
	state.player.textureID = LoadTexture("me.png");


	GLuint tileTextureID = LoadTexture("tile.png");
	GLuint appleTextureID = LoadTexture("apple.png");

	GLuint fontTextureID = LoadTexture("font1.png");

	std::string mission_successful = "Mission Successful";
	std::string mission_failed = "Mission Failed";

	state.success_text.entityType = TEXT;
	state.success_text.isActive = false;
	state.success_text.textureID = fontTextureID;
	state.success_text.text = mission_successful;
	state.success_text.position = glm::vec3(-4.2, 0, 0);

	state.failed_text.entityType = TEXT;
	state.failed_text.isActive = false;
	state.failed_text.textureID = fontTextureID;
	state.failed_text.text = mission_failed;
	state.failed_text.position = glm::vec3(-3.3, 0, 0);

	int ID = 0;
	float bottom_x = -5;
	float left_y = -2.25;
	float right_y = -2.25;

	//bottom border
	while (bottom_x <= 5) {
		state.platforms[ID].textureID = tileTextureID;
		state.platforms[ID].position = glm::vec3(bottom_x, -3.25f, 0);
		ID++;
		bottom_x += 1;
	}

	//left border
	while (left_y <= 3.75) {
		state.platforms[ID].textureID = tileTextureID;
		state.platforms[ID].position = glm::vec3(-5, left_y, 0);
		ID++;
		left_y += 1;
	}

	//right border
	while (right_y <= 3.75) {
		state.platforms[ID].textureID = tileTextureID;
		state.platforms[ID].position = glm::vec3(5, right_y, 0);
		ID++;
		right_y += 1;
	}

	//barriers
	state.platforms[25].textureID = tileTextureID;
	state.platforms[25].position = glm::vec3(-4, 1.5, 0);
	state.platforms[26].textureID = tileTextureID;
	state.platforms[26].position = glm::vec3(-3, 1.5, 0);
	state.platforms[27].textureID = tileTextureID;
	state.platforms[27].position = glm::vec3(-2.5, 1.5, 0);
	state.platforms[28].textureID = tileTextureID;
	state.platforms[28].position = glm::vec3(-1.8, 0.5, 0);
	state.platforms[29].textureID = tileTextureID;
	state.platforms[29].position = glm::vec3(0.6, 0.2, 0);
	state.platforms[30].textureID = tileTextureID;
	state.platforms[30].position = glm::vec3(-1.8, -0.5, 0);
	state.platforms[31].textureID = tileTextureID;
	state.platforms[31].position = glm::vec3(-1.6, -1.5, 0);
	state.platforms[32].textureID = tileTextureID;
	state.platforms[32].position = glm::vec3(0.7, 1.2, 0);
	state.platforms[33].textureID = tileTextureID;
	state.platforms[33].position = glm::vec3(1.6, 0.2, 0);

	//goal
	state.platforms[34].textureID = appleTextureID;
	state.platforms[34].position = glm::vec3(0.4, -2.25f, 0);
	state.platforms[34].entityType = GOAL;


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
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;
		}
	}

	state.player.velocity.x = 0;
	// Check for pressed/held keys below
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_A] && (!state.player.gameEnd))
	{
		state.player.acceleration.x = -40.0f;
	}
	else if (keys[SDL_SCANCODE_D] && (!state.player.gameEnd))
	{
		state.player.acceleration.x = 40.0f;
	}
	else if (keys[SDL_SCANCODE_R])
	{
		//restart game
		state.player.position = glm::vec3(-2, 3, 0);
		state.player.acceleration = glm::vec3(0.0f, -0.70f, 0.0f);
		state.player.gameEnd = false;
		state.player.collideGoal = false;
		state.success_text.isActive = false;
		state.failed_text.isActive = false;
	}
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 2000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {
		// Update. Notice it's FIXED_TIMESTEP. Not deltaTime
		state.player.Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);

		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;
}


void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	state.player.Render(&program);

	for (int i = 0; i < PLATFORM_COUNT; i++) {
		state.platforms[i].Render(&program);
	}
	state.success_text.Render(&program);
	state.failed_text.Render(&program);

	SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		if (state.player.gameEnd) {
			state.player.velocity = glm::vec3(0, 0, 0);
			state.player.acceleration = glm::vec3(0, 0, 0);
			if (state.player.collideGoal) {
				state.success_text.isActive = true;
			}
			else {
				state.failed_text.isActive = true;
			}
		}
		ProcessInput();
		Update();
		Render();
	}

	Shutdown();
	return 0;
}