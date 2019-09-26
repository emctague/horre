# horre

Horre is an attempt at developing a small, first-person game engine and horror game in C++.

It is important to note that horre is *not* designed with modern game engine programming patterns in mind.
It does *not* use an Entity Component System, a modern graphics library like Vulkan or DX12, or anything else you might
expect from a modern engine. It uses OpenGL 3.3, and for the time being most entity behavior is crammed into a single
catch-all Entity class. 

## How to I run this thing?

- Install GLFW on your system in such a way that `-lglfw` can be used to link.
  Or just modify `CMakeLists.txt` to find it, I guess.
  (And yes, I *know* that isn't how modern CMake is supposed to be used! I'm just being lazy!)
  
- Install Assimp on your system so that it can be included and linked to.

- Install GLM somewhere such that programs can `#include <glm/...>`. 

- Make a build directory

- `cd build`

- `cmake ..`

- `make`

- `./horre`

Note that the horre binary CANNOT work without the `test` directory being present in the directory above the current
working directory you run it from, at least for the time being. It refers to resources internally with paths like
`.../test/test.frag`.
