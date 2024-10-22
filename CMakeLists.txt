cmake_minimum_required(VERSION 3.5)

project(ClothSimulation LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    main.cpp
    src/glad.c
    window/*
    imgui/*
    tools/*
    opengl/*
    models/*
    shaders/*)
add_executable(ClothSimulation ${SOURCES} )
target_include_directories(ClothSimulation PUBLIC imgui include src window tools opengl models)

find_package(glfw3 3.3 REQUIRED)
# Find and include GLM
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
# Find and include GLFWa
find_package(glm REQUIRED)


file(GLOB GLSL_SOURCE_FILES
    shaders/*.frag
    shaders/*.vert
)

add_custom_target(
    Shaders
    DEPENDS ${GLSL_SOURCE_FILES}
)
add_dependencies(ClothSimulation Shaders)

add_custom_command(TARGET Shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "$<TARGET_PROPERTY:ClothSimulation,SOURCE_DIR>/shaders"
    "$<TARGET_PROPERTY:ClothSimulation,BINARY_DIR>/$<CONFIGURATION>/shaders"
)

file(GLOB TEX_SOURCE_FILES
    textures/*
)
add_custom_target(
    Textures
    DEPENDS ${TEX_SOURCE_FILES}
)
add_dependencies(ClothSimulation Textures)
add_custom_command(TARGET Textures POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "$<TARGET_PROPERTY:ClothSimulation,SOURCE_DIR>/textures"
    "$<TARGET_PROPERTY:ClothSimulation,BINARY_DIR>/$<CONFIGURATION>/textures"
)
target_link_libraries(ClothSimulation  OpenGL::GL glfw glm::glm ${GLFW3_LIBRARY})

install(TARGETS ClothSimulation
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
