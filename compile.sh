#!/bin/bash

# Compiles this project

clang++ Application.cpp src/glad.c src/glErrorHandling.cpp src/VertexBuffer.cpp src/IndexBuffer.cpp  src/VertexArray.cpp src/Shader.cpp src/Renderer.cpp src/Shapes/Line.cpp src/Shapes/Fractal.cpp src/ScaleRotationMatrix.cpp src/Input.cpp src/FractalSkeleton.cpp src/Camera.cpp src/FractalRegion.cpp src/State.cpp src/WindowResizeCallback.cpp src/WindowCreator.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lOpenGL
