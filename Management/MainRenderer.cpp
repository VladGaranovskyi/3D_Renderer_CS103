#include "MainRenderer.h"
#include <SDL2/SDL.h>

MainRenderer::MainRenderer(int w, int h): window(nullptr), renderer(nullptr), width(w), height(h) {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}
MainRenderer::~MainRenderer() {
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
}
void MainRenderer::Clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}
void MainRenderer::ShowModel() {
	SDL_RenderPresent(renderer);
}
void MainRenderer::DrawPixel(int x, int y) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}
void MainRenderer::DrawLine(int x1, int y1, int x2, int y2) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
#include <MainRenderer.h>

