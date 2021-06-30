#include "src/glad.h"
// #include "GL/gl.h"
// #include "GL/freeglut.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
// #include "src/glfw/glfw-3.3.2/include/GLFW/glfw3.h"

#include "include/linmath/linmath.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#include "dependencies/glErrorHandling.h"
#define _DEBUG

#include "src/VertexBuffer.h"
#include "src/IndexBuffer.h"
#include "src/VertexArray.h"
#include "src/Shader.h"
#include "src/Renderer.h"
// #include "src/MouseButtonCallback.h"
// #include "src/DrawnLines.h"
#include "src/Input.h"
#include "src/FractalSkeleton.h"
#include "src/Camera.h"
#include "src/FractalRegion.h"
#include "src/WindowCreator.h"
#include "src/WindowResizeCallback.h"
#include "src/State.h"

#include "src/Shapes/Line.h"
#include "src/Shapes/Fractal.h"

#include <vector>


int main(int argc, char **argv)
{
  GLFWwindow* window;

  // const char* description;
  // int code = glfwGetError(&description);
  //
  // // if (description)
  //   std::cout << code << '\n';
      // display_error_message(code, description);

  glfwInit();

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); // looks interesting

  // int windowW, windowH;
  // windowW = 1920;
  // windowH = 1080;
  // window = glfwCreateWindow(1920, 1080, "Fractal Drawer", glfwGetPrimaryMonitor(), NULL);
  // window = glfwCreateWindow(windowW, windowH, "Fractal Drawer", glfwGetPrimaryMonitor(), NULL);
  window = glfwCreateWindow(1280, 720, "Fractal Drawer", NULL, NULL);

  if (!window) {
    std::cout << "No GLFW window context, terminating" << '\n';
    glfwTerminate();
    return -1;
  }


  int windowW, windowH;
  glfwGetWindowSize(window, &windowW, &windowH);
  int fBufferW, fBufferH;
  glfwGetFramebufferSize(window, &fBufferW, &fBufferH);
  std::cout << "/* message */ " << windowW << " x " << windowH << " | " << fBufferW << " x " << fBufferH << '\n';

  glfwMakeContextCurrent(window);


  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }


  glfwSwapInterval(1); // limits framerate
  // glViewport(0, 0, windowW, windowH);


  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';

  #ifdef _DEBUG
    if(glDebugMessageCallback){
      std::cout << "Register OpenGL debug callback " << std::endl;
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(openglErrorHandlingCallback, nullptr);
      GLuint unusedIds = 0;
      glDebugMessageControl(GL_DONT_CARE,
          GL_DONT_CARE,
          GL_DONT_CARE,
          0,
          &unusedIds,
          true);
    }
    else
      std::cout << "glDebugMessageCallback not available" << std::endl;
  #endif




  VertexArray va;

  float positions[] = {};
  unsigned int indices[] = {};

  VertexBuffer vb(positions, 0 * sizeof(float));
  IndexBuffer ibLines(indices, (unsigned int)6);

  VertexBufferLayout layout;
  layout.Push<float>(2);

  Renderer renderer;

  // Shader shader("/usr/res/Fractal Drawer/shaders/basic.shader");
  Shader shader("res/shaders/basic.shader");
  shader.Bind();
  // Shader monochromeShader("/usr/res/Fractal Drawer/shaders/monochrome.shader");
  Shader monochromeShader("res/shaders/monochrome.shader");
  monochromeShader.Bind();
  // Shader repeateFractalShader("/usr/res/Fractal Drawer/shaders/repeate_fractal.shader");
  Shader repeateFractalShader("res/shaders/repeate_fractal.shader");


  va.Unbind();
  vb.Unbind();
  ibLines.Unbind();
  shader.Unbind();
  monochromeShader.Unbind();


  float dotSize;
  dotSize = 10.0f;

  Camera camera;
  camera.ScaleToMatchScreen(windowW, windowH);
  Input::currentCamera = &camera;


  // glfwSetMouseButtonCallback(window, MouseButtonPressCallback);
  glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
  // glfwSetCursorPosCallback	(window, MouseMoveCallback);
  glfwSetKeyCallback(window, Input::KeyCallback);
  glfwSetWindowSizeCallback(window, WindowResizeCallback);


  glEnable(GL_POINT_SMOOTH);


  // serpinsky

  Line squareMainLine = {0.9, 0.9, -0.9, 0.9};
  std::vector<Line> squareBaseLines;
  Line baseLine1 = {0.9, 0.9, 0.0, 0.0};
  squareBaseLines.push_back(baseLine1);
  Line baseLine2 = {0.0, 0.0, -0.9, 0.9};
  squareBaseLines.push_back(baseLine2);
  std::vector<Line> squareDirectionLines;
  Line directionLine1 = {0.9, 0.9, 0.3, 0.9};
  Line directionLine2 = {0.3, 0.9, -0.3, 0.9};
  Line directionLine3 = {-0.9, 0.9, -0.9, 0.3};
  Line directionLine4 = {-0.9, 0.3, -0.9, -0.3};
  Line directionLine5 = {-0.3, -0.9, 0.3, -0.9};
  Line directionLine6 = {0.3, -0.9, 0.9, -0.9};
  Line directionLine7 = {0.9, -0.3, 0.9, 0.3};
  Line directionLine8 = {-0.9, -0.3, -0.9, -0.9};
  squareDirectionLines.push_back(directionLine1);
  squareDirectionLines.push_back(directionLine2);
  squareDirectionLines.push_back(directionLine3);
  squareDirectionLines.push_back(directionLine4);
  squareDirectionLines.push_back(directionLine5);
  squareDirectionLines.push_back(directionLine6);
  squareDirectionLines.push_back(directionLine7);
  squareDirectionLines.push_back(directionLine8);
  FractalSkeleton serpinskySquareSkeleton;
  serpinskySquareSkeleton.mainLine = squareMainLine;
  serpinskySquareSkeleton.directionLines = squareDirectionLines;
  serpinskySquareSkeleton.baseLines = squareBaseLines;
  Fractal serpinskySquare(squareMainLine, squareBaseLines, squareDirectionLines, 10);
  Fractal serpinskySquareBigger(squareMainLine, squareBaseLines, squareDirectionLines, 100000);

  VertexBuffer f_s_vb(serpinskySquare.vertices, 4 * serpinskySquare.linesCount * sizeof(float));
  IndexBuffer f_s_ib(serpinskySquare.indices, (unsigned int)serpinskySquare.linesCount * 2);

  VertexBuffer f_s_b_vb(serpinskySquareBigger.vertices, 4 * serpinskySquareBigger.linesCount * sizeof(float));
  IndexBuffer f_s_b_ib(serpinskySquareBigger.indices, (unsigned int)serpinskySquareBigger.linesCount * 2);

  Line triangleMainLine = {0.9, 0.9, -0.9, 0.9};
  std::vector<Line> triangleBaseLines;
  Line baseLine0_1 = {0.9, 0.9, -0.9, 0.9};
  triangleBaseLines.push_back(baseLine0_1);
  Line baseLine0_2 = {0.0, 0.0, -0.9, 0.9};
  // triangleBaseLines.push_back(baseLine0_2);
  std::vector<Line> triangleDirectionLines;
  Line directionLine0_1 = {0.45, 0.9 - 0.9 * 1.732050808 / 2, 0.9, 0.9};
  Line directionLine0_2 = {-0.45, 0.9 - 0.9 * 1.732050808 / 2, 0.0, 0.9 - 0.9 * 1.732050808};
  Line directionLine0_3 = {0.0, 0.9, -0.9, 0.9};
  triangleDirectionLines.push_back(directionLine0_1);
  triangleDirectionLines.push_back(directionLine0_2);
  triangleDirectionLines.push_back(directionLine0_3);
  FractalSkeleton serpinskyTriangleSkeleton;
  serpinskyTriangleSkeleton.mainLine = triangleMainLine;
  serpinskyTriangleSkeleton.directionLines = triangleDirectionLines;
  serpinskyTriangleSkeleton.baseLines = triangleBaseLines;
  Fractal serpinskyTriangle(triangleMainLine, triangleBaseLines, triangleDirectionLines);
  Fractal serpinskyTriangleBigger(triangleMainLine, triangleBaseLines, triangleDirectionLines, 90000);

  VertexBuffer f_t_vb(serpinskyTriangle.vertices, 4 * serpinskyTriangle.linesCount * sizeof(float));
  IndexBuffer f_t_ib(serpinskyTriangle.indices, (unsigned int)serpinskyTriangle.linesCount * 2);

  VertexBuffer f_t_b_vb(serpinskyTriangleBigger.vertices, 4 * serpinskyTriangleBigger.linesCount * sizeof(float));
  IndexBuffer f_t_b_ib(serpinskyTriangleBigger.indices, (unsigned int)serpinskyTriangleBigger.linesCount * 2);

  std::unique_ptr<FractalSkeleton> u_p_fs(new FractalSkeleton);
  Input::currentFractal = std::move(u_p_fs);
  Input::currentFractal->mainLine = {-0.3, -0.3, 0.3, 0.3};
  Line baseLine1_1 = {-0.3, -0.3, 0.3, 0.3};
  Input::currentFractal->baseLines.push_back(baseLine1_1);
  Line directionLine1_1 = {-0.3, -0.33, 0.3, -0.33};
  Line directionLine1_2 = {-0.3, 0.3, 0.3, 0.3};
  Input::currentFractal->directionLines.push_back(directionLine1_1);
  Input::currentFractal->directionLines.push_back(directionLine1_2);
  Input::currentFractal->depth = 9;

  FractalSkeleton dragonCurveSkeleton;
  dragonCurveSkeleton.mainLine = {-0.3, -0.3, 0.3, 0.3};
  dragonCurveSkeleton.baseLines.push_back(baseLine1_1);
  Line directionLine1_1_1 = {-0.3, -0.3, 0.3, -0.3};
  dragonCurveSkeleton.directionLines.push_back(directionLine1_1_1);
  dragonCurveSkeleton.directionLines.push_back(directionLine1_2);

  std::vector<Fractal> userDrawnFractals;
  std::vector<FractalRegion> userDrawnFractalRegions;
  std::vector<std::unique_ptr<VertexBuffer>> userDrawnFractalsVB;
  std::vector<std::unique_ptr<IndexBuffer>> userDrawnFractalsIB;


  Line fractalMainLine = {0.0, 0.9, 0.0, 0.0};
  std::vector<Line> fractalBaseLines;
  Line baseLine2_1 = {0.0, 0.9, 0.0, 0.0};
  fractalBaseLines.push_back(baseLine2_1);
  // Line baseLine2_2 = {0.0, 0.0, -0.9, 0.9};
  // fractalBaseLines.push_back(baseLine2_2);
  std::vector<Line> fractalDirectionLines;
  Line directionLine2_1 = {0.0, 0.0, 0.3, -0.3};
  Line directionLine2_2 = {0.0, 0.0, -0.3, -0.3};
  // Line directionLine2_3 = {0.0, 0.9, -0.9, 0.9};
  fractalDirectionLines.push_back(directionLine2_1);
  fractalDirectionLines.push_back(directionLine2_2);
  // fractalDirectionLines.push_back(directionLine2_3);
  Fractal fractal(fractalMainLine, fractalBaseLines, fractalDirectionLines);

  VertexBuffer f_vb(fractal.vertices, 4 * fractal.linesCount * sizeof(float));
  IndexBuffer f_ib(fractal.indices, (unsigned int)fractal.linesCount * 2);

  FractalRegion fractalRegion(&serpinskyTriangle);

  // camera.ZoomToCoordinates(0.0, -0.1, 6.0);
  // std::cout << "0 - expanding fractals count: " << fractalRegion.expandingFractals.size() << '\n';
  fractalRegion.UpdateProcessibleReflectionLinesIndices(camera);
  fractalRegion.CreateNesessaryExpandingFractals();
  fractalRegion.UpdateProcessibleReflectionLinesIndices(camera);
  fractalRegion.CreateNesessaryExpandingFractals();
  fractalRegion.UpdateProcessibleReflectionLinesIndices(camera);
  fractalRegion.CreateNesessaryExpandingFractals();
  fractalRegion.UpdateProcessibleReflectionLinesIndices(camera);
  fractalRegion.CreateNesessaryExpandingFractals();

  // float* ef_vertices = new float[fractalRegion.expandingFractals.size() * serpinskySquare.lastReflectionLinesCount * 2 * 2];
  // unsigned int* ef_indices = new unsigned int[fractalRegion.expandingFractals.size() * serpinskySquare.lastReflectionLinesCount * 2];
  //
  // for (size_t i = 0; i < fractalRegion.expandingFractals.size(); i++) {
  //   for (size_t j = 0; j < serpinskySquare.lastReflectionLinesCount; j++) {
  //     ef_vertices[(i * serpinskySquare.lastReflectionLinesCount + j) * 4  + 0] = fractalRegion.expandingFractals[i].reflectionLines[j].x0;
  //     ef_vertices[(i * serpinskySquare.lastReflectionLinesCount + j) * 4  + 1] = fractalRegion.expandingFractals[i].reflectionLines[j].y0;
  //     ef_vertices[(i * serpinskySquare.lastReflectionLinesCount + j) * 4  + 2] = fractalRegion.expandingFractals[i].reflectionLines[j].x1;
  //     ef_vertices[(i * serpinskySquare.lastReflectionLinesCount + j) * 4  + 3] = fractalRegion.expandingFractals[i].reflectionLines[j].y1;
  //     ef_indices[(i * serpinskySquare.lastReflectionLinesCount + j) * 2  + 0] = (i * serpinskySquare.lastReflectionLinesCount + j) * 2  + 0;
  //     ef_indices[(i * serpinskySquare.lastReflectionLinesCount + j) * 2  + 1] = (i * serpinskySquare.lastReflectionLinesCount + j) * 2  + 1;
  //   }
  // }
  //
  // VertexBuffer ef_vb(ef_vertices, 4 * fractalRegion.expandingFractals.size() * serpinskySquare.lastReflectionLinesCount * sizeof(float));
  // IndexBuffer ef_ib(ef_indices, (unsigned int)fractalRegion.expandingFractals.size() * serpinskySquare.lastReflectionLinesCount * 2);


  // std::cout << "ef rl : " << fractalRegion.expandingFractals[0].reflectionLines[0].x0 << " " << fractalRegion.expandingFractals[0].reflectionLines[0].y0 << " " << fractalRegion.expandingFractals[0].reflectionLines[0].x1 << " " << fractalRegion.expandingFractals[0].reflectionLines[0].y1 << " " << '\n';
  std::cout << "ef size: " << fractalRegion.expandingFractals.size() << '\n';
  std::cout << "serpinskyTriangle lastReflectionLinesCount: " << serpinskyTriangle.lastReflectionLinesCount << '\n';

  while (!glfwWindowShouldClose(window)) {
    renderer.Clear();

    // shader.SetUniform4f("u_MousePos", float(mouseX / windowW), float(mouseY / windowH), 0.0f, 0.0f);

    // va.AddBuffer(f_s_vb, layout);
    // renderer.DrawLines(va, f_s_ib, shader, 1.0f);

    // va.AddBuffer(f_t_vb, layout);
    // renderer.DrawLines(va, f_t_ib, shader, 1.0f);

    // va.AddBuffer(ef_vb, layout);
    // renderer.DrawLines(va, ef_ib, shader, 1.0f);

    // va.AddBuffer(f_vb, layout);
    // renderer.DrawFractalColored(fractal, va, f_ib, monochromeShader, camera, 1.0f, layout);

    // va.AddBuffer(f_t_b_vb, layout);
    // renderer.DrawFractalRegion(fractalRegion, va, f_t_b_ib, repeateFractalShader, camera, 1.0f, layout);


    // renderer.DrawLineSegments(va, shader, layout);

    // for (size_t i = 0; i < userDrawnFractals.size(); i++) {
    //   va.AddBuffer(*userDrawnFractalsVB[i], layout);
    //   // renderer.DrawLines(va, *userDrawnFractalsIB[i], shader, 1.0f);
    //   renderer.DrawFractalColored(userDrawnFractals[i], va, *userDrawnFractalsIB[i], monochromeShader, camera, 1.0f, layout);
    // }

    for (size_t i = 0; i < userDrawnFractalRegions.size(); i++) {
      va.AddBuffer(*userDrawnFractalsVB[i], layout);
      renderer.DrawFractalRegion(userDrawnFractalRegions[i], va, *userDrawnFractalsIB[i], repeateFractalShader, camera, 2.0f, layout);
    }

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    Fractal fractal = Input::currentFractal->GenerateFractalPreview(camera.x + camera.w * (mouseX / windowW), camera.y + camera.h * (- mouseY / windowH + 1.0));
    VertexBuffer f_vb(fractal.vertices, 4 * fractal.linesCount * sizeof(float));
    IndexBuffer f_ib(fractal.indices, (unsigned int)fractal.linesCount * 2);
    va.AddBuffer(f_vb, layout);
    renderer.DrawFractalColored(fractal, va, f_ib, monochromeShader, camera, 2.0f, layout);
    renderer.DrawFractalSkeleton(*(Input::currentFractal), va, shader, layout, 3.0f, camera);

    /* CHANGE glfwWaitEvents() TO glfwPollEvents() TO REMOVE FREEZES ON ABCENSE OF EVENTS & DECREASE DRAWING ACCURACY */

    glfwPollEvents();
    // glfwWaitEvents();

    if (Input::fullscreenIsReadyToBeToggled) {
      GLFWwindow* newWindow;
      if (State::windowIsFullscreen) {
        newWindow = WindowCreator::CreateWindowedWindow(1280, 720, window);
        State::windowIsFullscreen = false;
      } else {
        newWindow = WindowCreator::CreateFullscreenWindow(1920, 1080, window);
        State::windowIsFullscreen = true;
      }
      glfwDestroyWindow(window);
      window = newWindow;
      glfwGetWindowSize(window, &windowW, &windowH);

      Input::fullscreenIsReadyToBeToggled = false;
    }

    if (State::windowWasResized) {
      std::cout << State::windowSize.first << " " << State::windowSize.second << '\n';
      windowW = State::windowSize.first;
      windowH = State::windowSize.second;
      camera.ScaleToMatchScreen(windowW, windowH);
      glViewport(0, 0, windowW, windowH);
      State::windowWasResized = false;
    }

    if (Input::fractalsAreReadyToBeDeleted) {
      Input::fractalsAreReadyToBeDeleted = false;
      userDrawnFractals.clear();
      userDrawnFractalRegions.clear();
      userDrawnFractalsVB.clear();
      userDrawnFractalsIB.clear();
    }

    if (Input::cameraIsReadyToBeReset) {
      Input::cameraIsReadyToBeReset = false;
      camera.x = camera.y = -1.0;
      camera.w = camera.h = 2.0;
      camera.ScaleToMatchScreen(windowW, windowH);
    }

    if (Input::predrawnFractalIsReady > 0) {
      switch (Input::predrawnFractalIsReady) {
        case 1:
          Input::currentFractal->mainLine = serpinskySquareSkeleton.mainLine;
          Input::currentFractal->directionLines = serpinskySquareSkeleton.directionLines;
          Input::currentFractal->baseLines = serpinskySquareSkeleton.baseLines;
          break;
        case 2:
          Input::currentFractal->mainLine = serpinskyTriangleSkeleton.mainLine;
          Input::currentFractal->directionLines = serpinskyTriangleSkeleton.directionLines;
          Input::currentFractal->baseLines = serpinskyTriangleSkeleton.baseLines;
          break;
        case 3:
          Input::currentFractal->mainLine = dragonCurveSkeleton.mainLine;
          Input::currentFractal->directionLines = dragonCurveSkeleton.directionLines;
          Input::currentFractal->baseLines = dragonCurveSkeleton.baseLines;
          break;
      }
      Input::predrawnFractalIsReady = 0;
    }


    double cameraWBefore, cameraWAfter;

    if (Input::cameraZoom != 1) {
      double mouseX, mouseY;
      glfwGetCursorPos(window, &mouseX, &mouseY);
      cameraWBefore = camera.w;
      camera.ZoomToCoordinates((mouseX / windowW - 0.5) * 2, - (mouseY / windowH - 0.5) * 2, Input::cameraZoom);
      cameraWAfter = camera.w;
      if (!Input::cameraZoomLocked) {
        Input::cameraZoom = 1;
      }
    }

    if (std::floor(std::log(cameraWBefore) / std::log(1.5)) != std::floor(std::log(cameraWAfter) / std::log(1.5))) {
      for (size_t i = 0; i < userDrawnFractalRegions.size(); i++) {
        bool eFWereCreated = true;
        if (userDrawnFractalRegions[i].expandingFractals.size() > 100) {
          userDrawnFractalRegions[i].expandingFractals.clear();
          userDrawnFractalRegions[i].processibleReflectionLinesIndices.clear();
          userDrawnFractalRegions[i].expandingFractalsParentIndices.clear();
        }
        while (eFWereCreated) {
          userDrawnFractalRegions[i].UpdateProcessibleReflectionLinesIndices(camera);
          eFWereCreated = userDrawnFractalRegions[i].CreateNesessaryExpandingFractals();
        }
        std::cout << "expanding fractals count: " << userDrawnFractalRegions[i].expandingFractals.size() << '\n';
        for (size_t j = 0; j < userDrawnFractalRegions[i].expandingFractals.size(); j++) {
          std::cout << "layer: " << userDrawnFractalRegions[i].expandingFractals[j].layer << '\n';
        }
      }
    }


    if (Input::fractalIsReady) {
      Input::fractalIsReady = false;
      if (Input::currentFractal->directionLines.size() == 2) {
        userDrawnFractals.push_back(Input::currentFractal->GenerateFractal(14, 0.01));
        int lastIndex = userDrawnFractals.size() - 1;
        userDrawnFractalRegions.emplace_back(&userDrawnFractals[lastIndex]);
        Fractal biggerFractal = Input::currentFractal->GenerateFractal(300000);
        FractalRegion biggerFractalRegion(&biggerFractal);
        userDrawnFractalRegions[lastIndex].fractalWidth = biggerFractalRegion.fractalWidth;
        userDrawnFractalRegions[lastIndex].fractalHeight = biggerFractalRegion.fractalHeight;
        userDrawnFractalsVB.push_back( std::unique_ptr<VertexBuffer>(new VertexBuffer(biggerFractal.vertices, 4 * biggerFractal.linesCount * sizeof(float))) );
        userDrawnFractalsIB.push_back( std::unique_ptr<IndexBuffer> (new IndexBuffer(biggerFractal.indices, (unsigned int)biggerFractal.linesCount * 2)) );
      } else {
        userDrawnFractals.push_back(Input::currentFractal->GenerateFractal(14));
        int lastIndex = userDrawnFractals.size() - 1;
        userDrawnFractalRegions.emplace_back(&userDrawnFractals[lastIndex]);
        Fractal biggerFractal = Input::currentFractal->GenerateFractal(90000);
        FractalRegion biggerFractalRegion(&biggerFractal);
        userDrawnFractalRegions[lastIndex].fractalWidth = biggerFractalRegion.fractalWidth;
        userDrawnFractalRegions[lastIndex].fractalHeight = biggerFractalRegion.fractalHeight;
        userDrawnFractalsVB.push_back( std::unique_ptr<VertexBuffer>(new VertexBuffer(biggerFractal.vertices, 4 * biggerFractal.linesCount * sizeof(float))) );
        userDrawnFractalsIB.push_back( std::unique_ptr<IndexBuffer> (new IndexBuffer(biggerFractal.indices, (unsigned int)biggerFractal.linesCount * 2)) );
      }
      for (size_t i = 0; i < userDrawnFractalRegions.size(); i++) {
        userDrawnFractalRegions[i].fractal = &userDrawnFractals[i];
      }
      std::unique_ptr<FractalSkeleton> u_ptr_fs(new FractalSkeleton);
      Input::currentFractal = std::move(u_ptr_fs);
    }

    glfwSwapBuffers(window);

    glfwPollEvents();
    // glfwWaitEvents();
  }


  glfwTerminate();
}
