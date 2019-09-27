# horre

Horre is an attempt at developing a small, first-person game engine and horror game in C++.

It is important to note that horre is *not* designed with modern game engine programming patterns in mind.
It does *not* use an Entity Component System, a modern graphics library like Vulkan or DX12, or anything else you might
expect from a modern engine. It uses OpenGL 3.3, and for the time being most entity behavior is crammed into a single
catch-all Entity class. 

## How to I run this thing?

- Install GLFW, Assimp, and GLM on your system so that they can be found my CMake.

- Make a build directory

- `cd build`

- `cmake ..`

- `make`

- `./horre`

Note that the horre binary CANNOT work without the `res` directory being present in the directory above the current
working directory you run it from, at least for the time being. It refers to resources internally with paths like
`.../res/shader/test.frag`.

## Plans

- Deferred rendering needs to be fixed

- Maybe switch to vulkan? Might be a PITA especially on mac.

- We still need to add basic physics

- Load stuff from a map file instead of programmatically
