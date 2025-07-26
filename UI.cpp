#include "Cache.h"
Cache::Config cacheConfig;

#define DEBUG_INFO

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "SDL.h"

#ifdef _WIN32
#include <windows.h> // SetProcessDPIAware()
#endif

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif
// Main code
int main(int argc, char* argv[])
{
	// Setup SDL: não mexer!
#ifdef _WIN32
	::SetProcessDPIAware();
#endif
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with SDL_Renderer graphics context
	float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("CacheSimAS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(1280 * main_scale), (int)(720 * main_scale), window_flags);
	SDL_Surface* icon = SDL_LoadBMP("assets/icon.bmp");
	if(icon){
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}
	if(window == nullptr){
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if(renderer == nullptr){
		SDL_Log("Error creating SDL_Renderer!");
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Variables
	bool done = false;

	// Main loop
	while(!done){
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
		while(SDL_PollEvent(&event)){
			ImGui_ImplSDL2_ProcessEvent(&event);
			if(event.type == SDL_QUIT) done = true;
			if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) done = true;
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) done = true;

			#ifdef DEBUG_INFO
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d){
				std::cout << "cacheConfig.size = " << cacheConfig.size << std::endl;
				std::cout << "cacheConfig.block_size = " << cacheConfig.block_size << std::endl;
				std::cout << "cacheConfig.associativity = " << cacheConfig.associativity << std::endl;
				std::cout << "cacheConfig.policy = " << cacheConfig.policy << std::endl;
				std::cout << std::endl;
			}
			#endif
			
		}
		if(SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED){
			SDL_Delay(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Config window -> nsets, block_size, assoc, repl
		{
			// static float f = 0.0f;

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_Always);
			ImGui::Begin("CONFIG", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); // Create window

			// TODO: interação entre a config escolhida na UI e a config rodada pelo sim
			// if(componente) -> evento, faz coisas dentro do if
			
			ImGui::SetNextItemWidth(100);
			ImGui::InputInt("Size", &cacheConfig.size); // inplace changing the variable should have no consequences if the user is not a dumb fuck
			
			ImGui::SetNextItemWidth(100);
			ImGui::InputInt("Block size", &cacheConfig.block_size);

			ImGui::SetNextItemWidth(100);
			ImGui::InputInt("Associativity", &cacheConfig.associativity);

			ImGui::SetNextItemWidth(100);
			const char* policies[] = {"LRU", "FIFO", "RANDOM"};
			if(ImGui::BeginCombo("Replacement Policy", policies[cacheConfig.policy])){
				for(int i = 0; i < IM_ARRAYSIZE(policies); i++){
					bool is_selected = (cacheConfig.policy == static_cast<Cache::ReplacementPolicy>(i));
					if(ImGui::Selectable(policies[i], is_selected)) cacheConfig.policy = static_cast<Cache::ReplacementPolicy>(i);
					if(is_selected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Text("avg %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::Text("press Q to quit");
			ImGui::End();
		}

		// Rendering - não mexer!
		ImGui::Render();
		SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);
	}

	// Cleanup - não mexer!
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}