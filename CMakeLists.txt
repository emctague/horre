cmake_minimum_required(VERSION 3.14)
project(horre)


if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else ()
	set(CMAKE_CXX_STANDARD 17)
endif ()

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

if (MSVC)
	find_library(ASSIMP_DEBUG_LIBRARY assimp-vc140-mtd.lib)
	add_library(assimp INTERFACE)
	target_include_directories(assimp INTERFACE ${ASSIMP_INCLUDE_DIRS})
	target_link_libraries(assimp INTERFACE
		debug ${ASSIMP_DEBUG_LIBRARY}
		${ASSIMP_LIBRARIES}
)
endif ()

add_executable(horre src/main.cpp glad/src/glad.c src/glinc.h src/Shader.cpp src/Shader.h src/Model.cpp src/Model.h src/Entity.cpp src/Entity.h src/Window.cpp src/Window.h src/ResourceManager.h src/ResourceSet.h src/ResourceSet.cpp src/Texture.cpp src/Texture.h stb/stb_image.h src/Mesh.cpp src/Mesh.h src/GlobalConfig.h src/Framebuffer.cpp src/Framebuffer.h)
target_link_libraries(horre PRIVATE glfw assimp glm)
target_include_directories(horre PRIVATE glad/include)

add_custom_target(resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
        BYPRODUCTS ${CMAKE_BINARY_DIR}/res
        DEPENDS res)

add_dependencies(horre resources)

add_custom_command(TARGET horre POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E tar "cfvz" "horre.tgz" $<TARGET_FILE_DIR:horre>/res $<TARGET_FILE_DIR:horre>/horre)
