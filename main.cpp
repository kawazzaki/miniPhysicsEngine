#include<iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include "external/glm/glm/glm.hpp"

#include "imgui.h"
#include "backend/imgui_impl_sdl3.h"
#include "backend/imgui_impl_opengl3.h"

#include "Rendering/Renderer.h"
#include "Objects/Square.h"

#pragma comment(lib,"SDL3.lib")
#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"opengl32.lib")


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_Window* window = SDL_CreateWindow("PhysicsEngine", 800, 600, SDL_WINDOW_OPENGL);

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, glContext);

	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();

	//Imgui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330");

	Renderer renderer;
	renderer.init();

	Square square;
	square.body.position = { 0.0f,2.0f };
	square.size = 1.0f;
	square.color = { 0.9f, 0.4f, 0.2f };

	bool running = true;
	Uint64 lastTicks = SDL_GetTicksNS();
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT) running = false;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) running = false;
		}
		Uint64 nowTicks = SDL_GetTicksNS();
		float dt = (float)(nowTicks - lastTicks) / 1e9f;
		lastTicks = nowTicks;
		(void)dt;
		int w, h;
		SDL_GetWindowSizeInPixels(window, &w, &h);
		renderer.beginFrame(w, h);
		renderer.drawQuad(square.body.position, square.size, square.color);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug");
		ImGui::DragFloat2("Position", &square.body.position.x, 0.05f);
		ImGui::DragFloat("Size", &square.size, 0.05f, 0.1f, 5.0f);
		ImGui::ColorEdit3("Color", &square.color.x);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(window);

	}
	renderer.shutdown();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}