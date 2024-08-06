# Tea-Engine

<p align="center">
    <img src="/Resources/temporal-logo.webp?raw=true">
</p>

**Tea Engine is a Beautiful(not yet but it will be) and Cross-platform 3D Game Engine written in C++ using OpenGL and GLFW.** </br> 

> [!WARNING]
> This is a personal project for learning how to make a 3D game engine.

---

# Getting Started

Any IDE or Text Editor that supports CMake Projects should work for developing the project.

## Building

### 1. Clone the repository
```
git clone --recursive https://github.com/HugoPlacer/Tea-Engine.git
cd Tea-Engine
```

### 2. Compilation
<details>
  <summary>Windows</summary>
(Not Tested but you should use cmake with vcpkg (easiest way i think is with vs2022))
</details>
<details>
  <summary>Linux</summary>

#### 1. Install Dependencies
- Arch Linux
```
sudo pacman -S spdlog fmt glfw glm assimp
```
- Fedora
```
sudo dnf install spdlog-devel fmt-devel glfw-devel glm-devel assimp-devel
```
- Ubuntu
```
sudo apt-get install libspdlog-dev libfmt-dev libglfw3-dev libglm-dev libassimp-dev
```

#### 2. Build
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j $(nproc) TeaEditor
```
#### 3. Enjoy!
```
cd ../bin/TeaEditor/Release
./TeaEditor
```
</details>

---

# Features (WIP)
- [x] Compatible with Windows and Linux.
- [x] 3D Rendering
  - [x] Textures
  - [x] Materials
  - [x] Shaders
  - [ ] PBR Rendering
  - [ ] Lights
  - [ ] Environment
- [x] Scene System
  - [x] Scene Tree
  - [x] ECS System
  - [x] Model Loading
- [x] Editor
- [ ] Save And Loading

---

# Screenshots

![Tea-Engine Screenshot-1.png](/Resources/Screenshot-1.png?raw=true)

---

## Acknowledgements

This project makes use of several open-source libraries and resources that have greatly contributed to its development. Special thanks to the creators and maintainers of the following:

- **[spdlog](https://github.com/gabime/spdlog)**: Fast C++ logging library.
- **[GLFW](https://www.glfw.org/)**: Library for handling window creation, input, and events.
- **[GLM](https://github.com/g-truc/glm)**: OpenGL Mathematics library for graphics software.
- **[GLAD](https://github.com/Dav1dde/glad)**: Multi-language OpenGL loader-generator.
- **[ImGui](https://github.com/ocornut/imgui)**: Immediate Mode Graphical User Interface for C++.
- **[stb_image](https://github.com/nothings/stb)**: Image loading library.
- **[assimp](https://github.com/assimp/assimp)**: Library to import various well-known 3D model formats.
- **[EnTT](https://github.com/skypjack/entt)**: Fast and reliable entity-component system (ECS).
- **[Tracy](https://github.com/wolfpld/tracy)**: Real-time, nanosecond resolution, remote telemetry, hybrid frame and sampling profiler.
- **[LearnOpenGL](https://learnopengl.com/)**: Comprehensive OpenGL tutorial and resource site.
- **[Hazel](https://github.com/TheCherno/Hazel)**: A small game engine developed by TheCherno as part of his game engine series.
- **[Godot](https://godotengine.org/)**: An advanced, feature-packed, multi-platform 2D and 3D open-source game engine.
- **[skypjack on software](https://skypjack.github.io/)**: Blog and resources on software development by skypjack.
