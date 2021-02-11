cmake_minimum_required(VERSION 3.12.2)

project (fractal-drawer VERSION 1.2)

add_library(glad STATIC src/glad.c)
target_include_directories( glad
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(glErrorHandling STATIC src/glErrorHandling.cpp)
target_include_directories( glErrorHandling
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(IndexBuffer STATIC src/IndexBuffer.cpp)
target_include_directories( IndexBuffer
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(Renderer STATIC src/Renderer.cpp)
target_include_directories( Renderer
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(Shader STATIC src/Shader.cpp)
target_include_directories( Shader
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(VertexArray STATIC src/VertexArray.cpp)
target_include_directories( VertexArray
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(VertexBuffer STATIC src/VertexBuffer.cpp)
target_include_directories( VertexBuffer
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(Camera STATIC src/Camera.cpp)
target_include_directories( Camera
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(FractalRegion STATIC src/FractalRegion.cpp)
target_include_directories( FractalRegion
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(FractalSkeleton STATIC src/FractalSkeleton.cpp)
target_include_directories( FractalSkeleton
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(Input STATIC src/Input.cpp)
target_include_directories( Input
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(ScaleRotationMatrix STATIC src/ScaleRotationMatrix.cpp)
target_include_directories( ScaleRotationMatrix
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(Fractal STATIC src/Shapes/Fractal.cpp)
target_include_directories( Fractal
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
add_library(Line STATIC src/Shapes/Line.cpp)
target_include_directories( Line
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)



add_executable(fractal-drawer Application.cpp)
target_link_libraries(fractal-drawer glad)
target_link_libraries(fractal-drawer glErrorHandling)
target_link_libraries(fractal-drawer IndexBuffer)
target_link_libraries(fractal-drawer Renderer)
target_link_libraries(fractal-drawer Shader)
target_link_libraries(fractal-drawer VertexArray)
target_link_libraries(fractal-drawer VertexBuffer)
target_link_libraries(fractal-drawer Camera)
target_link_libraries(fractal-drawer FractalRegion)
target_link_libraries(fractal-drawer FractalSkeleton)
target_link_libraries(fractal-drawer Input)
target_link_libraries(fractal-drawer ScaleRotationMatrix)
target_link_libraries(fractal-drawer Fractal)
target_link_libraries(fractal-drawer Line)

option(LINK_GLFW_STATICALLY "Build glfw? (set no, if you have glfw installed)" OFF)

if (LINK_GLFW_STATICALLY)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(src/glfw/glfw-3.3.2)
else ()
find_package(glfw3 3.2 REQUIRED)
endif ()

target_link_libraries(fractal-drawer glfw)

find_package(OpenGL REQUIRED)
# include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(fractal-drawer OpenGL::GL)

target_link_libraries(fractal-drawer ${CMAKE_DL_LIBS})


install(DIRECTORY "${CMAKE_SOURCE_DIR}/res/" DESTINATION "res/Fractal Drawer/")

install(TARGETS fractal-drawer
    COMPONENT linapp
    RUNTIME DESTINATION "bin/"
    LIBRARY DESTINATION "bin/"
    DESTINATION "bin/"
    PERMISSIONS WORLD_EXECUTE
)

# install(FILES fractalDrawer.sh
#     DESTINATION "bin/"
#     PERMISSIONS WORLD_EXECUTE
# )

install(FILES "Fractal Drawer.desktop" DESTINATION /usr/share/applications )

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sbd") #required
INCLUDE(CPack)
