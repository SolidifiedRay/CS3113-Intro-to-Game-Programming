#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

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
ShaderProgram textured_program;

glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Entity left, right, ball;
Entity leftwin, rightwin;

GLuint fontTextureID;

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

bool HitTopBorder(Entity* object) {
	return (object->position.y + (object->height) / 2) >= 3.65;
}

bool HitBottomBorder(Entity* object) {
	return (object->position.y - (object->height) / 2) <= -3.65;
}

bool HitRightBorder(Entity* object) {
	return (object->position.x + (object->width) / 2) >= 4.9;
}

bool HitLeftBorder(Entity* object) {
	return (object->position.x - (object->width) / 2) <= -4.9;
}

bool HitEntity(Entity* object1, Entity* object2) {
	float x_distance = fabs(object2->position.x - object1->position.x) - ((object1->width + object2->width) / 2.0f);
	float y_distance = fabs(object2->position.y - object1->position.y) - ((object1->height + object2->height) / 2.0f);

	return (x_distance < 0.2 && y_distance < 0.2);
}

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	left.position = glm::vec3(-4.8f, 0.0f, 0.0f);
	left.movement = glm::vec3(0, 0, 0);
	right.position = glm::vec3(4.8f, 0.0f, 0.0f);
	right.movement = glm::vec3(0, 0, 0);
	ball.position = glm::vec3(0.0f, 0.0f, 0.0f);
	ball.movement = glm::vec3(1.5f, -0.3f, 0);

	leftwin.position = glm::vec3(-4.3f, 3.0f, 0.0f);
	rightwin.position = glm::vec3(4.3f, 3.0f, 0.0f);

	program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
	textured_program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	leftwin.textureID = LoadTexture("win.png");
	rightwin.textureID = LoadTexture("win.png");

	left.speed = 2;
	right.speed = 2;
	ball.speed = 2;

	left.height = 2.1;
	left.width = 0.25;
	right.height = 2.1;
	right.width = 0.25;
	ball.height = 0.42;
	ball.width = 0.42;

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

	textured_program.SetProjectionMatrix(projectionMatrix);
	textured_program.SetViewMatrix(viewMatrix);

	glUseProgram(program.programID);
	glUseProgram(textured_program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}


void ProcessInput() {
	left.movement = glm::vec3(0, 0, 0);
	right.movement = glm::vec3(0, 0, 0);
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
				// Some sort of action
				break;

			}
			break;
		}
	}
	// Check for pressed/held keys below
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W])
	{
		if (HitTopBorder(&left)) {
			left.movement.y = 0;
		}
		else {
			left.movement.y = 1;
		}
	}
	else if (keys[SDL_SCANCODE_S])
	{
		if (HitBottomBorder(&left)) {
			left.movement.y = 0;
		}
		else {
			left.movement.y = -1;
		}
	}

	if (glm::length(left.movement) > 1.0f)
	{
		left.movement = glm::normalize(left.movement);
	}

	if (keys[SDL_SCANCODE_UP])
	{
		if (HitTopBorder(&right)) {
			right.movement.y = 0;
		}
		else {
			right.movement.y = 1;
		}
	}
	else if (keys[SDL_SCANCODE_DOWN])
	{
		if (HitBottomBorder(&right)) {
			right.movement.y = 0;
		}
		else {
			right.movement.y = -1;
		}
	}

	if (glm::length(right.movement) > 1.0f)
	{
		right.movement = glm::normalize(right.movement);
	}
}

float lastTicks = 0;

int winner = 0;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	if (HitEntity(&ball, &right)) {
		ball.movement.x *= -1;
	}
	else if (HitEntity(&ball, &left)) {
		ball.movement.x *= -1;
	}
	else if (HitTopBorder(&ball)) {
		ball.movement.y *= -1;
	}
	else if (HitBottomBorder(&ball)) {
		ball.movement.y *= -1;
	}
	else if (HitLeftBorder(&ball)) {
		ball.position = glm::vec3(0.0f, 0.0f, 0.0f);
		ball.movement = glm::vec3(-1.3f, -0.3f, 0);
		left.position = glm::vec3(-4.8f, 0.0f, 0.0f);
		right.position = glm::vec3(4.8f, 0.0f, 0.0f);
		winner = 2;
	}
	else if (HitRightBorder(&ball)) {
		ball.position = glm::vec3(0.0f, 0.0f, 0.0f);
		ball.movement = glm::vec3(1.3f, -0.3f, 0);
		left.position = glm::vec3(-4.8f, 0.0f, 0.0f);
		right.position = glm::vec3(4.8f, 0.0f, 0.0f);
		winner = 1;
	}


	if (ball.position.x > 7.0f || ball.position.x < -7.0f || ball.position.y > 5.0f || ball.position.y < -5.0f) {
		ball.position = glm::vec3(0.0f, 0.0f, 0.0f);
		ball.movement = glm::vec3(1.3f, -0.3f, 0);
		left.position = glm::vec3(-4.8f, 0.0f, 0.0f);
		right.position = glm::vec3(4.8f, 0.0f, 0.0f);
		winner = 0;
	}

	left.Update(deltaTime);
	right.Update(deltaTime);
	ball.Update(deltaTime*2);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, left.position);
	modelMatrix = glm::translate(modelMatrix, right.position);
	modelMatrix = glm::translate(modelMatrix, ball.position);
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	float left_vertices[] = { -0.1, -1.0, 0.1, -1.0, 0.1, 1.0, -0.1, -1.0, 0.1, 1.0, -0.1, 1.0 };
	float right_vertices[] = { -0.1, -1.0, 0.1, -1.0, 0.1, 1.0, -0.1, -1.0, 0.1, 1.0, -0.1, 1.0 };
	float ball_vertices[] = { -0.2, -0.2, 0.2, -0.2, 0.2, 0.2, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2 };
	float win_vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	left.Render(&program, left_vertices);
	right.Render(&program, right_vertices);
	ball.Render(&program, ball_vertices);
	if (winner == 1) {
		leftwin.Render(&textured_program, win_vertices);
	}
	else if (winner == 2) {
		rightwin.Render(&textured_program, win_vertices);
	}

	SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		ProcessInput();
		Update();
		Render();
	}

	Shutdown();
	return 0;
}