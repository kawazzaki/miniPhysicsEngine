#include "Rendering/Renderer.h"

#define GLEW_STATIC
#include <GL/glew.h>


#include "external/glm/glm/gtc/matrix_transform.hpp"
#include "external/glm/glm/gtc/type_ptr.hpp"


#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"opengl32.lib")

static const char* VS = R"(#version 330 core
layout(location=0) in vec2 aPos;
uniform mat4 uMVP;
void main(){ gl_Position = uMVP * vec4(aPos, 0.0, 1.0); })";

static const char* FS = R"(#version 330 core
uniform vec3 uColor;
out vec4 FragColor;
void main(){ FragColor = vec4(uColor, 1.0); })";




static unsigned int compile(GLenum type, const char* src) {
	unsigned int s = glCreateShader(type);
	glShaderSource(s, 1, &src, nullptr);
	glCompileShader(s);
	return s;
}


bool Renderer::init() {
	unsigned int vs = compile(GL_VERTEX_SHADER, VS);
	unsigned int fs = compile(GL_FRAGMENT_SHADER, FS);

	m_program = glCreateProgram();
	glAttachShader(m_program, vs);
	glAttachShader(m_program, fs);
	glLinkProgram(m_program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	m_uMVP = glGetUniformLocation(m_program, "uMVP");
	m_uColor = glGetUniformLocation(m_program, "uColor");

	float v[] = {
		-0.5f,-0.5f,  0.5f,-0.5f,  0.5f, 0.5f,
		-0.5f,-0.5f,  0.5f, 0.5f, -0.5f, 0.5f 
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(v),
		v,
		GL_STATIC_DRAW
		);
	glVertexAttribPointer(0,
		2,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(float),
		(void*)0
	);
	glEnableVertexAttribArray(0);
	return true;

}

void Renderer::beginFrame(int w, int h) {
	glViewport(0, 0, 800, 600);
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float halfH = 4.5f, halfW = halfH * (float)w / (float)h;
	m_projection = glm::ortho(-halfW, halfW, -halfH, halfH);
}

void Renderer::drawQuad(glm::vec2 c, float size, glm::vec3 col) {
	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(c, 0.0f));
	m = glm::scale(m, glm::vec3(size, size, 1.0f));
	glm::mat4 mvp = m_projection * m;
	glUseProgram(m_program);
	glUniformMatrix4fv(m_uMVP, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform3fv(m_uColor, 1, glm::value_ptr(col));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::shutdown() {
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteProgram(m_program);
}