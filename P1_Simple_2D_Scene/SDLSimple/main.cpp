#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram untextured_program;
ShaderProgram textured_program;

glm::mat4 viewMatrix, untexturedModelMatrix, movingModelMatrix, rotatingModelMatrix, projectionMatrix;

float moving_object = 3;

GLuint movingObjectID;
GLuint rotatingObjectID;

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
	displayWindow = SDL_CreateWindow("Simple 2D Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	//initialize untextured object
	untextured_program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

	viewMatrix = glm::mat4(1.0f);
	untexturedModelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	untextured_program.SetProjectionMatrix(projectionMatrix);
	untextured_program.SetViewMatrix(viewMatrix);
	untextured_program.SetColor(1.0f, 1.0f, 0.0f, 1.0f);

	//initialize textured objects
	textured_program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	movingModelMatrix = glm::mat4(1.0f);
	rotatingModelMatrix = glm::mat4(1.0f);

	movingObjectID = LoadTexture("animal.png");
	rotatingObjectID = LoadTexture("apple.png");

	textured_program.SetProjectionMatrix(projectionMatrix);
	textured_program.SetViewMatrix(viewMatrix);

	glUseProgram(untextured_program.programID);
	glUseProgram(textured_program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			gameIsRunning = false;
		}
	}
}

float lastTicks = 0;
float rotate_z = 0;


void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	moving_object -= 1.0f * deltaTime;
	rotate_z += 45.0 * deltaTime;

	movingModelMatrix = glm::mat4(1.0f);
	movingModelMatrix = glm::translate(movingModelMatrix, glm::vec3(moving_object, 0.0f, 0.0f));


	rotatingModelMatrix = glm::mat4(1.0f);
	rotatingModelMatrix = glm::rotate(rotatingModelMatrix, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));

}

void Render() {
	
	glClear(GL_COLOR_BUFFER_BIT);

	//render untextured object
	untextured_program.SetModelMatrix(untexturedModelMatrix);

	float untextured_vertices[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
	glVertexAttribPointer(untextured_program.positionAttribute, 2, GL_FLOAT, false, 0, untextured_vertices);
	glEnableVertexAttribArray(untextured_program.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(untextured_program.positionAttribute);

	//render rotating object
	textured_program.SetModelMatrix(rotatingModelMatrix);

	float rotating_vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float rotating_texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, rotatingObjectID);

	glVertexAttribPointer(textured_program.positionAttribute, 2, GL_FLOAT, false, 0, rotating_vertices);
	glEnableVertexAttribArray(textured_program.positionAttribute);

	glVertexAttribPointer(textured_program.texCoordAttribute, 2, GL_FLOAT, false, 0, rotating_texCoords);
	glEnableVertexAttribArray(textured_program.texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(textured_program.positionAttribute);
	glDisableVertexAttribArray(textured_program.texCoordAttribute);

	//render moving object
	textured_program.SetModelMatrix(movingModelMatrix);

	float moving_vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float moving_texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, movingObjectID);

	glVertexAttribPointer(textured_program.positionAttribute, 2, GL_FLOAT, false, 0, moving_vertices);
	glEnableVertexAttribArray(textured_program.positionAttribute);

	glVertexAttribPointer(textured_program.texCoordAttribute, 2, GL_FLOAT, false, 0, moving_texCoords);
	glEnableVertexAttribArray(textured_program.texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(textured_program.positionAttribute);
	glDisableVertexAttribArray(textured_program.texCoordAttribute);

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
