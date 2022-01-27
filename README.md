# WallG Engine

## Installation
 - WallG Engine is a 3D game engine that I built using DirectX 11 and C++ 19. It was an important learning experience over several semesters at LaSalle College Vancouver. In each course, I completed the construction of a low-level game engine system. The graphics system uses DirectX 11 and custom HLSL shaders to render models with animations. These are imported with a self-built tool using assimp 5.0. This tool reads .fbx files extracts all of the meshes, animations, and material textures, and converts them into a format that's used within the WallG Engine. The WallG Engine uses an object component system similar to Unity as the basis for creating projects and experiments, such as my Dynamic Universe project. 


##Frameworks of the engine

- **Core**
  - Implemented the window initialization using Win 32 API
  - Implemented Block Allocator to manage a pool of preallocated blocks for fast allocation/deallocation and to prevent memory fragmentation

- **Math**
  - Implemented data structure for 3D and 2D graphic calculation include:
    * Vector 2,3,4
    * Matric 3,4
    * Quaternion
    
  - Implemented help struct for the engine to use include:
    * Circle, AABB, OBB, Ray, Random

- **Graphics**
  - Camera: implemented 2d and 3d camera in this engine
  - Material: implemented "Colors", "LightTypes", "Material", "Sampler", "texture", "textureType" for material to use 
  - Mash: implemented mash buffer which allows engine can load different mash. also, it can create some mesh by default including Sphere, cube, plane, Cylinder, and Skybox.
  - Model: implemented model loader using assimp 5.0  
  - Animation: implemented animation loader and skeleton for model.
  - Render: implemented different render systems include: sprite render, texture render and simple draw with 2D and 3D 
  - Shader: implemented vertex shader and pixel shader to draw staff on the screen
  - Particle: implemented particle emitter to draw particle at this engine 

- **AI**
  - Implemented different pathfinding searches include A-Star Search, Dijkstra-Search, Depth-First Search, Breadth-First Search
  - Implemented Finite State Machine for AI 
  - Implemented different AI behaviour include Steering Behavior, Wander Behavior, and Separation Behavior
 
- **Physics**
  - Implemented physics world using Physics::Paticle to calculated
  - Implemented Constreains to simulate different physics work



##Project
- **Post Processing**
  - This engine allows the loading of different shaders by using different.fx file 

- **Character Animation**
  - This engine allows different models to load different animations

- **Physics**
  - This engine support physics 

- **Dynamic Universe** 

 - Dynamic Universe is a senior project that I started in my final term at LaSalle College. The project is inspired by a presentation on a game called Star Citizen. The goal is to implement a space economy simulation sandbox where different ships will have to gather resources and trade for upgrades. In the game universe, there are multiple planets with different resources and factories. Each ship has AI to decide the best action based on the current situation. For example, if the current price for iron is more expensive than copper, the ship will find the closest planet with iron and start mining. Or when the ship has enough currency, it will find the factory with the best cost to upgrade. All the game systems are built using the data-driven component system in the WallG game engine. There are still a lot of features to work on and I plan to continue to add to this project in the future.


#Integrated External Library
- [Assimp](https://github.com/assimp/assimp) -- For 3D model importing
- [Imgui](https://github.com/ocornut/imgui) -- For Debug UI
- [DirectXTK](https://github.com/microsoft/DirectXTK) -- For Graphic & audio
- [RapidJSON](https://github.com/Tencent/rapidjson) -- For Loading and saving JSON file
- [Mixamo](https://www.mixamo.com/#/) -- For 3D model
