#pragma once


#include "external/glm/glm/glm.hpp"




struct PhysicsBody2D {
	glm::vec2 position{ 0.0f };
	glm::vec2 velocity{ 0.0f };
	glm::vec2 acceleration{ 0.0f };
	float mass = 1.0f;
};