cmake_minimum_required(VERSION 3.14)
project(horre)

set(CMAKE_CXX_STANDARD 17)

add_executable(horre src/main.cpp glad/src/glad.c src/glinc.h src/Shader.cpp src/Shader.h src/Model.cpp src/Model.h src/Entity.cpp src/Entity.h src/Window.cpp src/Window.h src/ResourceManager.h src/ResourceSet.h src/ResourceSet.cpp src/Texture.cpp src/Texture.h stb/stb_image.h src/Mesh.cpp src/Mesh.h)
target_link_libraries(horre glfw assimp)
target_include_directories(horre PRIVATE glad/include)
