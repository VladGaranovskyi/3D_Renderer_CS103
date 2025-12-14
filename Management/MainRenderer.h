#ifndef MAINRENDERER_H
#define MAINRENDERER_H

#include <SDL2/SDL.h>

class MainRenderer {
public:
    MainRenderer(int width, int height);
    ~MainRenderer();

    void Clear();
    void ShowModel();
    void DrawPixel(int x, int y);
    void DrawLine(int x1, int y1, int x2, int y2);
    void SetColor(int r, int g, int b);
    void DrawHorizontalLine(int x1, int x2, int y);

    int GetWidth() const  { 
        return width; 
    }
    
    int GetHeight() const { 
        return height; 
    }

    SDL_Window* GetWindow() const{
        return window;
    }

    SDL_Renderer* GetRenderer() const{
        return renderer;
    }

private:
    SDL_Window*   window;
    SDL_Renderer* renderer;
    int width;
    int height;
};
#endif