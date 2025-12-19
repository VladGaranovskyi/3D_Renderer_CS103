#include "MainRenderer.h"
#include <SDL2/SDL.h>

MainRenderer::MainRenderer(int w, int h): window(nullptr), renderer(nullptr), width(w), height(h) {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, 0);
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

void MainRenderer::SetColor(int r, int g, int b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void MainRenderer::DrawHorizontalLine(int x1, int x2, int y) {
	if (x1 > x2) {
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}
	for (int x = x1; x <= x2; x++) {
		SDL_RenderDrawPoint(renderer, x, y);
	}
}

void MainRenderer::Resize(int w, int h){
	width = w;
	height = h;
	SDL_RenderSetLogicalSize(renderer, width, height);
}

