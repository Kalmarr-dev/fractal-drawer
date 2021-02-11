#include "FractalRegion.h"


ExpandingFractal::ExpandingFractal(Fractal* f, Line mainLine, int layer) {
  ExpandingFractal::mainLine = mainLine;
  ExpandingFractal::layer = layer;

  ExpandingFractal::srm = ScaleRotationMatrix(f->mainLine, mainLine);
  // ExpandingFractal::translationVector = {0.0, 0.0, mainLine.x0 - f->mainLine.x0, mainLine.y0 - f->mainLine.y0};
  // ExpandingFractal::translationVector = {0.0, 0.0, 0.0, 0.0};
  // ExpandingFractal::translationVector = srm.MultiplyByVector(ExpandingFractal::translationVector);

  ExpandingFractal::reflectionLinesCount = f->lastReflectionLinesCount;
  ExpandingFractal::reflectionLines = new Line[ExpandingFractal::reflectionLinesCount];
  for (size_t i = 0; i < ExpandingFractal::reflectionLinesCount; i++) {
    Line xy0 = {f->mainLine.x0, f->mainLine.y0, f->lastReflectionLines[i].x0, f->lastReflectionLines[i].y0};
    Line xy1 = {f->mainLine.x0, f->mainLine.y0, f->lastReflectionLines[i].x1, f->lastReflectionLines[i].y1};
    Line _xy0 = ExpandingFractal::srm.MultiplyByVector(xy0);
    Line _xy1 = ExpandingFractal::srm.MultiplyByVector(xy1);
    Line reflectionLine = {_xy0.x1 /* + ExpandingFractal::translationVector.x1 */ + mainLine.x0,
                           _xy0.y1 /* + ExpandingFractal::translationVector.y1 */ + mainLine.y0,
                           _xy1.x1 /* + ExpandingFractal::translationVector.x1 */ + mainLine.x0,
                           _xy1.y1 /* + ExpandingFractal::translationVector.y1 */ + mainLine.y0};
    ExpandingFractal::reflectionLines[i] = reflectionLine;
  }
}

ExpandingFractal::~ExpandingFractal() {
  delete[] reflectionLines;
}

ExpandingFractal::ExpandingFractal(ExpandingFractal&& f)
  : mainLine(f.mainLine),
    layer(f.layer),
    reflectionLinesCount(f.reflectionLinesCount),
    reflectionLines(f.reflectionLines),
    translationVector(f.translationVector),
    srm(f.srm)
{
  f.reflectionLines = nullptr;
}


FractalRegion::FractalRegion(Fractal* fractal_ptr) : fractal(fractal_ptr) {
  double minX, maxX;
  minX = maxX = fractal->vertices[0];
  for (int i = 0; i < fractal->linesCount * 2; i+=2) {
    minX = minX > fractal->vertices[i] ? fractal->vertices[i] : minX;
    maxX = maxX > fractal->vertices[i] ? maxX : fractal->vertices[i];
  }
  double minY, maxY;
  minY = maxY = fractal->vertices[0];
  for (int i = 1; i < fractal->linesCount * 2; i+=2) {
    minY = minY > fractal->vertices[i] ? fractal->vertices[i] : minY;
    maxY = maxY > fractal->vertices[i] ? maxY : fractal->vertices[i];
  }

  FractalRegion::fractalWidth = maxX - minX;
  FractalRegion::fractalHeight = maxY - minY;

  FractalIsStrictlyDecreasing = true;

  for (size_t i = 0; i < fractal->directionLinesCount; i++) {
    if (fractal->mainLine.GetLengthSquared() < fractal->directionLines[i].GetLengthSquared())
      FractalIsStrictlyDecreasing = false;
  }
}


void FractalRegion::UpdateProcessibleReflectionLinesIndices(const Camera& camera, double acceptableScreenSize) {
  processibleReflectionLinesIndices.clear();

  if (!FractalIsStrictlyDecreasing) return;

  double acceptableScreenSizeSquared = acceptableScreenSize * acceptableScreenSize;

  int currentIndex = 0;

  for (int i = 0; i < fractal->lastReflectionLinesCount; i++) {
    double distance = std::abs(camera.x - fractal->lastReflectionLines[i].x0) + std::abs(camera.y - fractal->lastReflectionLines[i].y0);
    double acceptableDistance = (camera.w + camera.h + fractalWidth + fractalHeight) * 2;
    if (distance < acceptableDistance && fractal->lastReflectionLines[i].GetLengthSquared() / (camera.w + camera.h) > acceptableScreenSizeSquared) {
      processibleReflectionLinesIndices.push_back(currentIndex);
    }
    currentIndex++;
  }


  double fractalMainLineLengthSquared = fractal->mainLine.GetLengthSquared();

  for (size_t i = 0; i < expandingFractals.size(); i++) {
    for (size_t j = 0; j < expandingFractals[i].reflectionLinesCount; j++) {
      double scalingFactorSquared = expandingFractals[i].reflectionLines[j].GetLengthSquared() / fractalMainLineLengthSquared;
      // double acceptableDistanceSquared = ((camera.w + camera.h) * (camera.w + camera.h) + (fractalWidth + fractalHeight) * (fractalWidth + fractalHeight) * scalingFactorSquared) * 2;
      double acceptableDistanceSquared = (std::max(camera.w, camera.h) * std::max(camera.w, camera.h) + std::max(fractalWidth, fractalHeight) * std::max(fractalWidth, fractalHeight) * scalingFactorSquared) * 2;
      double distance = (std::abs(camera.x - expandingFractals[i].reflectionLines[j].x0) + std::abs(camera.y - expandingFractals[i].reflectionLines[j].y0)) / 1.4;
      double distanceSquared = distance * distance;
      if (distanceSquared < acceptableDistanceSquared && expandingFractals[i].reflectionLines[j].GetLengthSquared() / (camera.w + camera.h) / (camera.w + camera.h) > acceptableScreenSizeSquared) {
        processibleReflectionLinesIndices.push_back(currentIndex);
      }
      currentIndex++;
    }
  }
}


bool FractalRegion::CreateNesessaryExpandingFractals() {
  bool ExpandingFractalsWereCreated = false;

  // int currentIndex = 0;
  // int currentIndexInProcessibleReflectionLinesIndices = 0;

  // std::sort(processibleReflectionLinesIndices.begin(), processibleReflectionLinesIndices.end());
  std::sort(expandingFractalsParentIndices.begin(), expandingFractalsParentIndices.end());

  if (processibleReflectionLinesIndices.size() == 0) {
    std::cout << "processibleReflectionLinesIndices.size() == 0" << '\n';
    return false;
  }

  for (int i = 0; i < processibleReflectionLinesIndices.size(); i++) {
    bool shouldBeAdded = true;
    if (binary_search(expandingFractalsParentIndices.begin(), expandingFractalsParentIndices.end(), processibleReflectionLinesIndices[i])) {
      continue;
    }
    // for (int j = 0; j < expandingFractalsParentIndices.size(); j++) {
    //   if (processibleReflectionLinesIndices[i] == expandingFractalsParentIndices[j]) {
    //     shouldBeAdded = false;
    //     break;
    //   }
    // }

    if (shouldBeAdded) {
      expandingFractalsParentIndices.push_back(processibleReflectionLinesIndices[i]);
      ExpandingFractalsWereCreated = true;
      if (processibleReflectionLinesIndices[i] >= fractal->lastReflectionLinesCount) {
        int c = processibleReflectionLinesIndices[i] / fractal->lastReflectionLinesCount - 1;
        expandingFractals.emplace_back(fractal, expandingFractals[c].reflectionLines[processibleReflectionLinesIndices[i] % fractal->lastReflectionLinesCount], expandingFractals[c].layer + 1);
      } else {
        std::cout << " " << fractal->lastReflectionLines[i].x0 << " " << fractal->lastReflectionLines[i].y0 << " " << fractal->lastReflectionLines[i].x1 << " " << fractal->lastReflectionLines[i].y1 << '\n';
        expandingFractals.emplace_back(fractal, fractal->lastReflectionLines[processibleReflectionLinesIndices[i]]);
      }
    }
  }


  if (ExpandingFractalsWereCreated) {
    maxExpandingFractalLayer = 0;
    for (size_t i = 0; i < expandingFractals.size(); i++) {
      maxExpandingFractalLayer = maxExpandingFractalLayer > expandingFractals[i].layer ? maxExpandingFractalLayer : expandingFractals[i].layer;
    }
  }

  // for (int i = 0; i < fractal->lastReflectionLinesCount; i++) {
  //   if (currentIndex < processibleReflectionLinesIndices[currentIndexInProcessibleReflectionLinesIndices]) {
  //
  //   } else if (currentIndex == processibleReflectionLinesIndices[currentIndexInProcessibleReflectionLinesIndices]) {
  //     // CreateNesessaryExpandingFractals
  //
  //     ExpandingFractalsWereCreated = true;
  //     currentIndexInProcessibleReflectionLinesIndices++;
  //   }
  //   currentIndex++;
  // }
  //
  // for (size_t i = 0; i < expandingFractals.size(); i++) {
  //   for (size_t j = 0; j < expandingFractals[i].reflectionLinesCount; j++) {
  //     if (currentIndex < processibleReflectionLinesIndices[currentIndexInProcessibleReflectionLinesIndices]) {
  //
  //     } else if (currentIndex == processibleReflectionLinesIndices[currentIndexInProcessibleReflectionLinesIndices]) {
  //       // CreateNesessaryExpandingFractals
  //
  //       ExpandingFractalsWereCreated = true;
  //       currentIndexInProcessibleReflectionLinesIndices++;
  //     }
  //     currentIndex++;
  //   }
  // }

  return ExpandingFractalsWereCreated;
}
