# mathos-opengl-terrain

# OpenGL 3D Terrain generator

An OpenGL application for procedurally generating terrain meshes using the Midpoint Displacement(Diamond-Square) algorithm. The application includes two shaders, a Phong lighting shader and a topological shader based on vertex height. 

### Application specifications:
* OpenGL v4.6
* GLSL v4.6

### External dependencies:
* [GLFW](https://www.glfw.org/)
* [GLAD, KHR](https://github.com/Dav1dde/glad)
* [GLM](https://glm.g-truc.net/0.9.9/index.html)
* [Dear ImGui](https://github.com/ocornut/imgui)
* [STB_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)

### System Requirements:
* Visual Studio 2019

### Running the program
1. Open the solution mathos-opengl-terrain/Mundus Novus/Mundus Novus.sln in Visual Studio 2019.
2. The target build options are Debug, x64.
![Build options](https://puu.sh/Gmlox/90fe9cdc9f.png)
3. Press Ctrl+F5 or *Debug->Start without debugging* to run the application. Building should recognise all the dependencies and successfully build the solution. 

### Controls
1. W, A, S, D - Camera movement
2. Right mouse button - Rotate camera
3. Esc - Exit application

### UI
[x] Wireframe - Enables wireframe view
[x] Topological shader - Enables topological shader instead of Phong lighting
* Seed - **integer**, seeds the pseudorandom number generator
* fHeight - **float**, Influences the minimum and maximum height during midpoint number generation
* fHeightReduction - **float [0, 1]**. Influences the random number dropoff during number generation. Represents terrain roughness.
* Generate - Generates terrain
* FPS Counter
* Controls (?) - Hover over to see controls tooltip

### Application Structure
* Application.cpp - main application file. Contains GLFW, GLAD initialisation, Window creation, input callbacks, main frame by frame rendering logic.
* Classes/
  * Camera - Camera properties and movement methods
  * Light - Light properties and movement methods. Passing light uniform values to shaders.
  * MidpointDisplacement - Heightmap manipulation and generation based on the *Midpoint Displacement* algorithm.
  * Shader - Creates shaders and handles passing information to shaders.
  * Terrain - Terrain information, setup, generation and drawing methods.
  * Texture - Loads a texture using STB_image.h
