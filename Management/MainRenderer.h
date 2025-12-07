#include <SDL2/SDL.h>

class MainRenderer {
public:
    MainRenderer(int width, int height);
    ~MainRenderer();

    void Clear();
    void ShowModel();
    void DrawPixel(int x, int y);
    void DrawLine(int x1, int y1, int x2, int y2);

    int GetWidth() const  { 
        return width; 
    }
    
    int GetHeight() const { 
        return height; 
    }

private:
    SDL_Window*   window;
    SDL_Renderer* renderer;
    int width;
    int height;
};
