Getting Started
===============

Any IDE or Text Editor that supports CMake Projects should work for
developing the project.

Building
========

1. Clone the repository
~~~~~~~~~~~~~~~~~~~~~~~

::

   git clone --recursive https://github.com/HugoPlacer/Coffee-Engine.git
   cd Coffee-Engine

2. Compilation
~~~~~~~~~~~~~~

.. raw:: html

   <details>

.. raw:: html

   <summary>

Windows

.. raw:: html

   </summary>

(Not Tested but you should use cmake with vcpkg (easiest way i think is
with vs2022))

.. raw:: html

   </details>

.. raw:: html

   <details>

.. raw:: html

   <summary>

Linux

.. raw:: html

   </summary>

1. Install Dependencies
^^^^^^^^^^^^^^^^^^^^^^^

-  Arch Linux

::

   sudo pacman -S spdlog fmt glfw glm assimp

-  Fedora

::

   sudo dnf install spdlog-devel fmt-devel glfw-devel glm-devel assimp-devel

-  Ubuntu

::

   sudo apt-get install libspdlog-dev libfmt-dev libglfw3-dev libglm-dev libassimp-dev

2. Build
^^^^^^^^

::

   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make -j $(nproc) CoffeeEditor

3. Enjoy!
^^^^^^^^^