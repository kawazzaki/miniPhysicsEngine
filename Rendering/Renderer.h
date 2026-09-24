#pragma once

#include "external/glm/glm/glm.hpp"


class Renderer {
public:
	bool init();
	void shutdown();
	void beginFrame(int width, int height);
	void drawQuad(glm::vec2 center, float size, glm::vec3 color);
private:
	unsigned int m_program, m_vao, m_vbo;
	glm::mat4 m_projection{ 1.0f };
	int m_uMVP = -1, m_uColor = -1;
};