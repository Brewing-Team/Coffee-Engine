List of Features
================

.. admonition:: Work In Progress

	This document is a work in progress and will be updated as the project progresses.

Platforms
---------

**Using CMake, the project is cross-platform and can be compiled on the following operating systems:**

- Windows
- Linux

**It should be possible to compile the project on other platforms as well, but it has not been tested and we don't have plans to do it.**

- Android
- iOS

**Not Supported Platforms:**

- MacOS (Fuck you Apple for deprecating OpenGL 🖕)

Project Structure
-----------------

- Build System
- Cross platform
- Core separado de la aplicación. Es decir que es una libreria separada.
- Organización de código. Como está estructurado el proyecto?
	- Abstracción de las librerías. OpenGL, SDL3, etc...
	- Code Patterns
	- Loop
	- Módulos
- Gestión del proyecto (Kanban + Github)

Debugging Tools
---------------

- Tracy

Arquitectura y Módulos
----------------------

- Layer System
- Event Dispatcher System
- Multiplatform System Information Class
-  Colored Logging System by Severity
- Extra Utils (Timer, Stopwatch)

Renderer
--------

- Debug Renderer
- Forward Renderer
- PBR Rendering (IBL WIP)
- Cubemaps
- Shaders combinados
- Materiales
- Texturas
- Modelos
- Render Batching
- Octree Static Frustum Culling
- Loose Octree Dynamic Frustum Culling (TODO)
- Camera
- Lighting
- HDR Rendering
- Tonemapping (Agx)
- OpenGL Abstraction
	- Vertex Array
	- Vertex Buffer
	- Index Buffer
	- Shader
	- Framebuffers
	- Texture2D
	- Cubemap
	- Uniform Buffers
	- Buffer Layouts
- Bounding Boxes (AABB y OBB)

Resource Management
-------------------

- Resource Registry
- Resource Loader + Importer
- Resource Format
- Resource Caching
- File Watcher TODO
- Embedded Assets
- Metadata
- UUID

Proyectos y Escenas
-------------------

- Proyectos
- Escenas
	- ECS
		- Explicación
		- Lista de Componentes
	- Primitives
- Serialización

Editor
------

- Mouse picking
- Log Panel (Editor)
- Drag and Drop (Editor)
- highDPI Support
- Editor Camera
	- Orbit Mode
	- FPS Mode
- Editor Features

Work In Progress and Planned Features
-------------------------------------

- Runtime Application
- Scripting
- Arquitectura para soportar más lenguajes de scripting