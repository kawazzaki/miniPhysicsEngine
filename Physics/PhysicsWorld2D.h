#pragma once

#include <vector>

#include "external/glm/glm/glm.hpp"
#include "PhysicsBody2D.h"


class PhysicsWorld2D {
public:
	glm::vec2 gravity{ 0.0f, -9.81f };
	std::vector<PhysicsBody2D*> bodies;

    void addBody(PhysicsBody2D* body) {
        bodies.push_back(body);
    }

    void step(float dt) {
        for (PhysicsBody2D* body : bodies) {
            body->acceleration = gravity;   
            body->integrate(dt);
        }
    }
};