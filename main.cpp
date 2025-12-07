#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char** argv) {
    // if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    //     std::cerr << "SDL Init Error: " << SDL_GetError() << "\n";
    //     return 1;
    // }

    // SDL_Window* window = SDL_CreateWindow(
    //     "SDL2 Test",
    //     SDL_WINDOWPOS_CENTERED,
    //     SDL_WINDOWPOS_CENTERED,
    //     800, 600,
    //     SDL_WINDOW_SHOWN
    // );

    // if (!window) {
    //     std::cerr << "Window Error: " << SDL_GetError() << "\n";
    //     SDL_Quit();
    //     return 1;
    // }

    // bool running = true;
    // SDL_Event event;

    // while (running) {
    //     while (SDL_PollEvent(&event)) {
    //         if (event.type == SDL_QUIT) {
    //             running = false;
    //         }
    //     }

    //     SDL_Delay(16); // ~60 FPS idle
    // }

    // SDL_DestroyWindow(window);
    // SDL_Quit();
    // return 0;

    /*
    Main Program:
    -initialize render, camera, load .obj file(make an interface for file input)
    -start a loop to: handle events, update (rotation or zoom in), (render.Clear() => model.Draw() => render.ShowModel())
    */
}

// Citations:
/*
https://en.wikipedia.org/wiki/3D_projection#Perspective_projection
https://wiki.libsdl.org/SDL2/CategoryRender
https://en.wikipedia.org/wiki/Rotation_matrix
https://www.youtube.com/watch?v=kdRJgYO1BJM
https://learnopengl.com/Getting-Started/Coordinate-Systems

*/
