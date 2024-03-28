cmake_minimum_required(VERSION 2.8)

project(COSC363_Assignment_1_swi126)

set(OpenGL_GL_PREFERENCE "LEGACY")

add_executable(Scene.out Scene.cpp Models.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( Scene.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
