#include "Entity.h"

Entity::Entity()
{
	position = glm::vec3(0);
	speed = 0;
	animTime = 0;
	animIndex = 0;
}

void Entity::Update(float deltaTime)
{

	if (movement.x == 0 && movement.y == 0) return;

	position += movement * speed * deltaTime;

	animTime += deltaTime;

	if (animTime >= 0.25f)
	{
		animTime = 0;
		animIndex++;
		if (animIndex >= animFrames)
		{
			animIndex = 0;
		}
	}
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, int index)
{
	float u = (float)(index % cols) / (float)cols;
	float v = (float)(index / cols) / (float)rows;

	float width = 1.0f / (float)cols;
	float height = 1.0f / (float)rows;

	float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
		u, v + height, u + width, v, u, v };

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	if (movement.x < 0) {
		DrawSpriteFromTextureAtlas(program, walkLeft[animIndex]);
	}
	else {
		DrawSpriteFromTextureAtlas(program, walkRight[animIndex]);
	}
}