#include "Entity.h"

Entity::Entity()
{
	entityType = PLATFORM;
	isStatic = true;
	isActive = true;
	restart = false;
	position = glm::vec3(0);
	speed = 0;
	width = 2;
	height = 1;
	life = 5;
	dead = false;
	shakeEffect = false;
	win = false;
	jump = Mix_LoadWAV("shoot.wav");
	hit = Mix_LoadWAV("hit.wav");
	origin_y = 0;
	origin_x = 0;
	boss_life = 15;
}

void Entity::ShootBullet(Entity player) {
	isActive = true;
	position = glm::vec3(player.position.x + 1.5, player.position.y, player.position.z); ;
	velocity.x = 20.0f;
	acceleration.x = 10.0f;
	Mix_PlayChannel(-1, jump, 0);
}

void Entity::BossShootBullet(Entity boss) {
	isActive = true;
	position = glm::vec3(boss.position.x - 2, boss.position.y, boss.position.z); ;
	velocity.x = -10.0f;
	acceleration.x = -10.0f;
	Mix_PlayChannel(-1, jump, 0);
}

bool Entity::CheckCollision(Entity other)
{
	if (isStatic == true) return false;
	if (isActive == false || other.isActive == false) return false;

	float xdist = fabs(position.x - other.position.x) - ((width + other.width) / 2.0f);
	float ydist = fabs(position.y - other.position.y) - ((height + other.height) / 2.0f);

	if (xdist < 0 && ydist < 0)
	{
		return true;
	}

	return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity object = objects[i];

		if (CheckCollision(object))
		{
			float ydist = fabs(position.y - object.position.y);
			float penetrationY = fabs(ydist - (height / 2) - (object.height / 2));
			if (velocity.y > 0) {
				position.y -= penetrationY;
				velocity.y = 0;
				collidedTop = true;
				if (entityType == PLAYER && object.entityType == ENEMY) {
					shakeEffect = true;
					if (life > 0) {
						life -= 1;
						restart = true;
						if (life == 0) {
							isActive = false;
							dead = true;
						}
					}
					else if (life == 0) {
						isActive = false;
						dead = true;
					}
				}
			}
			else if (velocity.y < 0) {
				position.y += penetrationY;
				velocity.y = 0;
				collidedBottom = true;
				if (entityType == PLAYER && object.entityType == ENEMY) {
					shakeEffect = true;
					objects[i].isActive = false;
				}
			}
		}
	}
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity object = objects[i];

		if (CheckCollision(object))
		{
			float xdist = fabs(position.x - object.position.x);
			float penetrationX = fabs(xdist - (width / 2) - (object.width / 2));
			if (velocity.x > 0) {
				position.x -= penetrationX;
				velocity.x = 0;
				collidedRight = true;
				if (entityType == PLAYER && object.entityType == ENEMY) {
					shakeEffect = true;
					if (life > 0) {
						life -= 1;
						restart = true;
						if (life == 0) {
							isActive = false;
							dead = true;
						}
					}
					else if (life == 0) {
						isActive = false;
						dead = true;
					}
				}
				if (entityType == BULLET && object.entityType == ENEMY && (object.aiState != BOSSAI)) {
					objects[i].isActive = false;
					isActive = false;
				}
				else if (entityType == BULLET && object.entityType == ENEMY && (object.aiState == BOSSAI)) {
					if (boss_life > 0) {
						boss_life -= 1;
						if (boss_life == 0) {
							objects[i].isActive = false;
						}
					}
					else if (boss_life == 0) {
						objects[i].isActive = false;
					}
					isActive = false;
				}
			}
			else if (velocity.x < 0) {
				position.x += penetrationX;
				velocity.x = 0;
				collidedLeft = true;
				if (entityType == PLAYER && object.entityType == ENEMY) {
					shakeEffect = true;
					if (life > 0) {
						life -= 1;
						restart = true;
						if (life == 0) {
							isActive = false;
							dead = true;
						}
					}
					else if (life == 0) {
						isActive = false;
					}
				}
				if (entityType == BULLET && object.entityType == ENEMY && (object.aiState != BOSSAI) ) {
					objects[i].isActive = false;
					isActive = false;
				}
				else if (entityType == BULLET && object.entityType == ENEMY && (object.aiState == BOSSAI)) {
					if (boss_life > 0) {
						boss_life -= 1;
						if (boss_life == 0) {
							objects[i].isActive = false;
						}
					}
					else if (boss_life == 0) {
						objects[i].isActive = false;
					}
					isActive = false;
				}
			}
		}
	}
}

void Entity::HitPlayer(Entity *player)
{
		Entity *object = player;

		if (CheckCollision(*object))
		{
			float xdist = fabs(position.x - (*object).position.x);
			float penetrationX = fabs(xdist - (width / 2) - ((*object).width / 2));
			if (velocity.x > 0) {
				position.x -= penetrationX;
				velocity.x = 0;
				collidedRight = true;
				if (entityType == BULLET && (*object).entityType == PLAYER) {
					shakeEffect = true; 
					isActive = false;
					if ((*object).life > 0) {
						(*object).life -= 1;
						restart = true;
						if ((*object).life == 0) {
							isActive = false;
							dead = true;
						}
					}
				}
			}
			else if (velocity.x < 0) {
				position.x += penetrationX;
				velocity.x = 0;
				collidedLeft = true;
				if (entityType == BULLET && (*object).entityType == PLAYER) {
					shakeEffect = true;
					isActive = false;
					if ((*object).life > 0) {
						(*object).life -= 1;
						restart = true;
						if ((*object).life == 0) {
							isActive = false;
							dead = true;
						}
					}
				}
			}
		}

}

void Entity::Jump()
{
	velocity.y = 4.0f;
}


void Entity::CheckCollisionsY(Map *map)
{
	// Probes for tiles
	glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
	glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
	glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

	glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
	glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
	glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

	float penetration_x = 0;
	float penetration_y = 0;
	if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}
	else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}
	else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
		position.y -= penetration_y;
		velocity.y = 0;
		collidedTop = true;
	}
	if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
	else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
	else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
		position.y += penetration_y;
		velocity.y = 0;
		collidedBottom = true;
	}
	if (collidedTop || collidedBottom) {
		if (life > 0) {
			life -= 1;
			restart = true;
			if (life == 0) {
				isActive = false;
				dead = true;
			}
		}
		else if (life == 0) {
			isActive = false;
		}
		if (entityType == BULLET) {
			isActive = false;
		}
	}
}

void Entity::CheckCollisionsX(Map *map)
{
	// Probes for tiles
	glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
	glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

	float penetration_x = 0;
	float penetration_y = 0;
	if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
		position.x += penetration_x;
		velocity.x = 0;
		collidedLeft = true;
	}

	if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
		position.x -= penetration_x;
		velocity.x = 0;
		collidedRight = true;
	}



	if (collidedLeft || collidedRight) {
		if (life > 0) {
			life -= 1;
			restart = true;
			if (life == 0) {
				isActive = false;
				dead = true;
			}
		}
		else if (life == 0) {
			isActive = false;
		}
		if (entityType == BULLET) {
			isActive = false;
		}
	}
}

void Entity::AI(Entity player, double y, double x) {
	double position_y = y;
	double position_x = x;
	switch (aiState) {
	case IDLE:
		if (glm::distance(position, player.position) < 4.0f) {
			aiState = WALKING;
		}
		break;
	case WALKING:
		if (player.position.x > position.x) {
			velocity.x = 1.0f;
		}
		else {
			velocity.x = -1.0f;
		}
		break;
	case UPDOWN:
		if (position.y - position_y == 0) {
			velocity.y = 1.0f;
		}
		if (position.y - position_y > 1) {
			velocity.y = -1.0f;
		}
		else if (position.y - position_y < -1) {
			velocity.y = 1.0f;
		}
		break;
	case BOSSAI:
		if (position.y - position_y == 0) {
			velocity.y = 2.0f;
			velocity.x = 2;
		}
		if (position.y - position_y > 3.5) {
			velocity.y = -2.0f;
		}
		else if (position.y - position_y < -3.5) {
			velocity.y = 2.0f;
		}
		break;
	}

}

void Entity::Update(float deltaTime, Entity *player, Entity* enemies, int enemyCount, Map *map, double origin_y, double origin_x)
{
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	velocity += acceleration * deltaTime;

	if (entityType == ENEMY) {
		AI(*player, origin_y, origin_x);
	}

	position.y += velocity.y * deltaTime; // Move on Y
	CheckCollisionsY(map);

	if (entityType == PLAYER) {
		CheckCollisionsY(enemies, enemyCount);
	}

	position.x += velocity.x * deltaTime; // Move on X
	CheckCollisionsX(map);

	if (entityType == PLAYER) {
		CheckCollisionsX(enemies, enemyCount);
	}

	if (entityType == BULLET) {
		CheckCollisionsX(enemies, enemyCount); 
		HitPlayer(player);
	}


}



void Entity::Render(ShaderProgram *program) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
