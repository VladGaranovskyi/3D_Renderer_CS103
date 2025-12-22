# 3D Renderer Project

A custom 3D rendering engine in C++ using SDL2 for graphics and ImGui for the user interface. This project implements core 3D graphics concepts including coordinate transformations, perspective projection, backface culling, and triangle rasterization.

## Features

- **OBJ File Loading**: Load and display 3D models in Wavefront .obj format
- **Real-time 3D Rendering**: Filled triangle rendering with wireframe overlay
- **Interactive Camera Controls**: 
  - Rotate camera with right mouse button + drag
  - Move camera with WASD + right mouse button
  - Adjustable zoom
- **Object Manipulation**:
  - Rotate objects with left mouse button + drag
  - Move objects with arrow keys
  - Adjustable position, rotation, and scale via UI
- **Inspector Mode**: Click on triangles to view detailed information (vertex positions, normals, depth)
- **Backface Culling**: Toggle face culling with normal flip option

## Controls

### Viewer Mode
- **Left Mouse + Drag**: Rotate object
- **Right Mouse + Drag**: Rotate camera
- **Right Mouse + WASD**: Move camera (forward/left/backward/right)
- **Right Mouse + Q/E**: Move camera (down/up)
- **Arrow Keys**: Move object in XY plane
- **+/- Keys**: Move object along Z axis

### Inspector Mode
- **Left Click**: Select triangle to inspect
- **Right Mouse + Drag**: Rotate object

## Building the Project

### Requirements
- C++17 or higher
- SDL2
- ImGui (included in project)
- Windows (for file dialog functionality)

### Compilation
```bash
g++ -std=c++17 main.cpp UIcode.cpp \
    Geometry/*.cpp Management/*.cpp \
    imgui/*.cpp imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_sdlrenderer2.cpp \
    -lSDL2 -lSDL2main -o renderer
```

## Project Structure

```
├── Geometry/           # 3D math and geometry classes
│   ├── Vector2/3       # Vector math operations
│   ├── Camera          # Camera transformations and projection
│   ├── Transform       # Local to world coordinate transforms
│   ├── Mesh            # Vertex and triangle storage
│   ├── Model           # Rendering and triangle building
│   └── ScreenTriangle  # 2D triangle with depth info
├── Management/         # File loading and rendering
│   ├── MainRenderer    # SDL2 wrapper
│   ├── ObjLoader       # .obj file parser
│   └── UserQuery       # File utilities
├── Objects/            # Directory for .obj model files
├── UIcode.cpp/h        # ImGui interface implementation
└── main.cpp            # Main application loop
```

## How It Works

1. **File Loading**: The ObjLoader parses .obj files, extracting vertices and faces, then converts polygons to triangles using fan triangulation
2. **Coordinate Transformation**: Objects are transformed from local space → world space → camera space → screen space
3. **Projection**: 3D points are projected onto a 2D screen using perspective projection
4. **Rasterization**: Triangles are filled using horizontal line interpolation
5. **Backface Culling**: Triangles facing away from the camera are optionally hidden using normal calculations


### Resources Used
- [Wikipedia: 3D Projection](https://en.wikipedia.org/wiki/3D_projection#Perspective_projection)
- [Wikipedia: Wavefront .obj File Format](https://en.wikipedia.org/wiki/Wavefront_.obj_file)
- [SDL2 Documentation](https://wiki.libsdl.org/SDL2/)
- [ImGui Documentation](https://github.com/ocornut/imgui)
- [Computer Graphics from Scratch - Triangle Rasterization](https://www.gabrielgambetta.com/computer-graphics-from-scratch/07-filled-triangles.html)
- [Backface Culling](https://medium.com/nerd-for-tech/optimizing-3d-rendering-with-backface-culling-5430e0821e0a)

## License

Educational project - free to use and modify.
