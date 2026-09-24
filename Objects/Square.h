#pragma once

#include "Physics/PhysicsBody2D.h"


struct Square {
	PhysicsBody2D body;
	float size = 1.0f;
	glm::vec3 color{ 1.0f };
};