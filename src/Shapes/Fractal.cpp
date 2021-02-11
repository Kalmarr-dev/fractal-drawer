#include "Fractal.h"
#include "../ScaleRotationMatrix.h"
#include <cmath>

Fractal::Fractal(const Line mainLine, const std::vector<Line>& baseLines, const std::vector<Line>& directionLines, const int MAXLINES, const float MINLINESIZE)
  : mainLine(mainLine),
  baseLinesCount(baseLines.size()),
  directionLinesCount(directionLines.size())
{

  int baseLinesSize = baseLines.size();
  int directionLinesSize = directionLines.size();

  Fractal::baseLines = new Line[baseLinesSize];
  for (int i = 0; i < baseLinesSize; i++) {
    Fractal::baseLines[i] = baseLines[i];
  }

  Fractal::directionLines = new Line[directionLinesSize];
  for (int i = 0; i < directionLinesSize; i++) {
    Fractal::directionLines[i] = directionLines[i];
  }

  ScaleRotationMatrix* MTBVectorsScaleRotationMatrices[baseLinesSize]; //main-to-base
  ScaleRotationMatrix* MTMTBVectorsScaleRotationMatrices[baseLinesSize]; //main-to-main-start-to-base-start
  ScaleRotationMatrix* MTDVectorsScaleRotationMatrices[directionLinesSize]; //main-to-direction
  ScaleRotationMatrix* MTMTDVectorsScaleRotationMatrices[directionLinesSize]; //main-to-main-start-to-direction-start

  for (int i = 0; i < baseLinesSize; i++) {
    MTBVectorsScaleRotationMatrices[i] = new ScaleRotationMatrix(mainLine, baseLines[i]);
  }

  for (int i = 0; i < baseLinesSize; i++) {
    Line MTBLine = {mainLine.x0, mainLine.y0, baseLines[i].x0, baseLines[i].y0};
    MTMTBVectorsScaleRotationMatrices[i] = new ScaleRotationMatrix(mainLine, MTBLine);
  }

  for (int i = 0; i < directionLinesSize; i++) {
    MTDVectorsScaleRotationMatrices[i] = new ScaleRotationMatrix(mainLine, directionLines[i]);
  }

  for (int i = 0; i < directionLinesSize; i++) {
    Line MTDLine = {mainLine.x0, mainLine.y0, directionLines[i].x0, directionLines[i].y0};
    MTMTDVectorsScaleRotationMatrices[i] = new ScaleRotationMatrix(mainLine, MTDLine);
  }


  Line* reflectionLines = new Line[10000000];
  reflectionLines[0] = mainLine;
  Fractal::linesOnTheLayer.push_back(1 * baseLinesSize);
  for (size_t i = 1; i < directionLinesSize + 1; i++) {
    reflectionLines[i] = directionLines[i - 1];
  }
  Fractal::linesOnTheLayer.push_back(directionLinesSize * baseLinesSize);

  std::vector<int> lastReflectionLinesIndices;


  int reflectionLinesOnPrevLayer = directionLinesSize;
  int reflectionLinesOnPrevLayers = 1 + reflectionLinesOnPrevLayer;
  int reflectionLinesOnThisLayer = 0;

  bool onThisLayerLinesWereCreated = true;

  while (reflectionLinesOnPrevLayers < MAXLINES && onThisLayerLinesWereCreated) {
    onThisLayerLinesWereCreated = false;

    for (int i = reflectionLinesOnPrevLayers - reflectionLinesOnPrevLayer; i < reflectionLinesOnPrevLayers; i++) {
      if (std::abs(reflectionLines[i].x1 - reflectionLines[i].x0) + std::abs(reflectionLines[i].y1 - reflectionLines[i].y0) < MINLINESIZE) {
        lastReflectionLinesIndices.push_back(i);
        continue;
      }
      for (int dLType = 0; dLType < directionLinesCount; dLType++) {
        int currentIndex = reflectionLinesOnPrevLayers + reflectionLinesOnThisLayer;
        Line startToStartVector = MTMTDVectorsScaleRotationMatrices[dLType]->MultiplyByVector(reflectionLines[i]);
        Line reflectionLineVector = MTDVectorsScaleRotationMatrices[dLType]->MultiplyByVector(reflectionLines[i]);
        double x0, y0, x1, y1;
        x0 = reflectionLines[i].x0 + startToStartVector.x1;
        y0 = reflectionLines[i].y0 + startToStartVector.y1;
        x1 = x0 + reflectionLineVector.x1;
        y1 = y0 + reflectionLineVector.y1;
        Line reflectionLine = {x0, y0, x1, y1};
        reflectionLines[currentIndex] = reflectionLine;
        reflectionLinesOnThisLayer++;
        onThisLayerLinesWereCreated = true;
      }
    }

    Fractal::linesOnTheLayer.push_back(reflectionLinesOnThisLayer * baseLinesSize);

    reflectionLinesOnPrevLayers += reflectionLinesOnThisLayer;
    reflectionLinesOnPrevLayer = reflectionLinesOnThisLayer;
    reflectionLinesOnThisLayer = 0;
  }


  for (int i = reflectionLinesOnPrevLayers - reflectionLinesOnPrevLayer; i < reflectionLinesOnPrevLayers; i++) {
    lastReflectionLinesIndices.push_back(i);
  }

  int reflectionLinesCount = reflectionLinesOnPrevLayers;

  // std::cout << "reflection lines count: " << reflectionLinesCount << '\n';

  Line* _reflectionLines = new Line[reflectionLinesCount];
  std::copy(reflectionLines, reflectionLines + reflectionLinesCount, _reflectionLines);
  delete[] reflectionLines;
  reflectionLines = _reflectionLines;

  Fractal::lastReflectionLinesCount = lastReflectionLinesIndices.size();
  Fractal::lastReflectionLines = new Line[Fractal::lastReflectionLinesCount];
  for (int i = 0; i < Fractal::lastReflectionLinesCount; i++) {
    Fractal::lastReflectionLines[i] = reflectionLines[lastReflectionLinesIndices[i]];
  }

  // std::cout << "Last reflection lines count: " << lastReflectionLinesCount << '\n';


  Fractal::linesCount = reflectionLinesCount * baseLinesSize;
  Fractal::lines = new Line[Fractal::linesCount];

  for (size_t i = 0; i < reflectionLinesCount; i++) {
    for (size_t j = 0; j < baseLinesSize; j++) {
      int currentIndex = i * baseLinesSize + j;
      Line startToStartVector = MTMTBVectorsScaleRotationMatrices[j]->MultiplyByVector(reflectionLines[i]);
      Line lineVector = MTBVectorsScaleRotationMatrices[j]->MultiplyByVector(reflectionLines[i]);
      double x0, y0, x1, y1;
      x0 = reflectionLines[i].x0 + startToStartVector.x1;
      y0 = reflectionLines[i].y0 + startToStartVector.y1;
      x1 = x0 + lineVector.x1;
      y1 = y0 + lineVector.y1;
      Line line = {x0, y0, x1, y1};
      Fractal::lines[currentIndex] = line;
    }
  }

  Fractal::vertices = new float[linesCount * 2 * 2];
  Fractal::indices = new unsigned int[linesCount * 2];

  for (int i = 0; i < linesCount; i++) {
    Fractal::vertices[i * 4 + 0] = lines[i].x0;
    Fractal::vertices[i * 4 + 1] = lines[i].y0;
    Fractal::vertices[i * 4 + 2] = lines[i].x1;
    Fractal::vertices[i * 4 + 3] = lines[i].y1;
    Fractal::indices[i * 2 + 0] = i * 2 + 0;
    Fractal::indices[i * 2 + 1] = i * 2 + 1;
  }

  // destructors
  for (int i = 0; i < baseLinesSize; i++) {
    delete MTBVectorsScaleRotationMatrices[i];
  }
  for (int i = 0; i < baseLinesSize; i++) {
    delete MTMTBVectorsScaleRotationMatrices[i];
  }
  for (int i = 0; i < directionLinesSize; i++) {
    delete MTDVectorsScaleRotationMatrices[i];
  }
  for (int i = 0; i < directionLinesSize; i++) {
    delete MTMTDVectorsScaleRotationMatrices[i];
  }
  delete[] reflectionLines;

}


// Fractal::Fractal(const Fractal& fractal, const Line mainLine) {
//   Fractal::mainLine = mainLine;
//
//   ScaleRotationMatrix srm(fractal.mainLine, mainLine);
//   Line translationVector {0.0, 0.0, mainLine.x0 - fractal.mainLine.x0, mainLine.y0 - fractal.mainLine.y0};
//
//   Fractal::baseLinesCount = fractal.baseLinesCount;
//   Fractal::baseLines = new Line[Fractal::baseLinesCount];
//   for (int i = 0; i < Fractal::baseLinesCount; i++) {
//     Line xy0 = {0.0, 0.0, fractal.baseLines[i].x0, fractal.baseLines[i].y0};
//     Line xy1 = {0.0, 0.0, fractal.baseLines[i].x1, fractal.baseLines[i].y1};
//     Line _xy0 = srm.MultiplyByVector(xy0);
//     Line _xy1 = srm.MultiplyByVector(xy1);
//     Line baseLine = {_xy0.x1 + translationVector.x1, _xy0.y1 + translationVector.y1, _xy1.x1 + translationVector.x1, _xy1.y1 + translationVector.y1};
//     Fractal::baseLines[i] = baseLine;
//   }
//
//   Fractal::directionLinesCount = fractal.directionLinesCount;
//   Fractal::directionLines = new Line[Fractal::directionLinesCount];
//   for (int i = 0; i < Fractal::directionLinesCount; i++) {
//     Line xy0 = {0.0, 0.0, fractal.directionLines[i].x0, fractal.directionLines[i].y0};
//     Line xy1 = {0.0, 0.0, fractal.directionLines[i].x1, fractal.directionLines[i].y1};
//     Line _xy0 = srm.MultiplyByVector(xy0);
//     Line _xy1 = srm.MultiplyByVector(xy1);
//     Line directionLine = {_xy0.x1 + translationVector.x1, _xy0.y1 + translationVector.y1, _xy1.x1 + translationVector.x1, _xy1.y1 + translationVector.y1};
//     Fractal::directionLines[i] = directionLine;
//   }
//
//   Fractal::linesCount = fractal.linesCount;
//   Fractal::lines = new Line[Fractal::linesCount];
//   for (int i = 0; i < Fractal::linesCount; i++) {
//     Line xy0 = {0.0, 0.0, fractal.lines[i].x0, fractal.lines[i].y0};
//     Line xy1 = {0.0, 0.0, fractal.lines[i].x1, fractal.lines[i].y1};
//     Line _xy0 = srm.MultiplyByVector(xy0);
//     Line _xy1 = srm.MultiplyByVector(xy1);
//     Line line = {_xy0.x1 + translationVector.x1, _xy0.y1 + translationVector.y1, _xy1.x1 + translationVector.x1, _xy1.y1 + translationVector.y1};
//     Fractal::lines[i] = line;
//   }
//
//   Fractal::baseLinesCount = fractal.baseLinesCount;
//   Fractal::baseLines = new Line[Fractal::baseLinesCount];
//   for (int i = 0; i < Fractal::baseLinesCount; i++) {
//     Line xy0 = {0.0, 0.0, fractal.baseLines[i].x0, fractal.baseLines[i].y0};
//     Line xy1 = {0.0, 0.0, fractal.baseLines[i].x1, fractal.baseLines[i].y1};
//     Line _xy0 = srm.MultiplyByVector(xy0);
//     Line _xy1 = srm.MultiplyByVector(xy1);
//     Line baseLine = {_xy0.x1 + translationVector.x1, _xy0.y1 + translationVector.y1, _xy1.x1 + translationVector.x1, _xy1.y1 + translationVector.y1};
//     Fractal::baseLines[i] = baseLine;
//   }
//
//   Fractal::lastReflectionLinesCount = fractal.lastReflectionLinesCount;
//   Fractal::lastReflectionLines = new Line[Fractal::lastReflectionLinesCount];
//   for (int i = 0; i < Fractal::lastReflectionLinesCount; i++) {
//     Line xy0 = {0.0, 0.0, fractal.lastReflectionLines[i].x0, fractal.lastReflectionLines[i].y0};
//     Line xy1 = {0.0, 0.0, fractal.lastReflectionLines[i].x1, fractal.lastReflectionLines[i].y1};
//     Line _xy0 = srm.MultiplyByVector(xy0);
//     Line _xy1 = srm.MultiplyByVector(xy1);
//     Line lastReflectionLine = {_xy0.x1 + translationVector.x1, _xy0.y1 + translationVector.y1, _xy1.x1 + translationVector.x1, _xy1.y1 + translationVector.y1};
//     Fractal::lastReflectionLines[i] = lastReflectionLine;
//   }
//
//   for (int i = 0; i < fractal.linesOnTheLayer.size(); i++) {
//     Fractal::linesOnTheLayer.push_back(fractal.linesOnTheLayer[i]);
//   }
//
//   Fractal::vertices = new float[linesCount * 2 * 2];
//   Fractal::indices = new unsigned int[linesCount * 2];
//
//   for (int i = 0; i < linesCount; i++) {
//     Fractal::vertices[i * 4 + 0] = Fractal::lines[i].x0;
//     Fractal::vertices[i * 4 + 1] = Fractal::lines[i].y0;
//     Fractal::vertices[i * 4 + 2] = Fractal::lines[i].x1;
//     Fractal::vertices[i * 4 + 3] = Fractal::lines[i].y1;
//     Fractal::indices[i * 2 + 0] = i * 2 + 0;
//     Fractal::indices[i * 2 + 1] = i * 2 + 1;
//   }
// }



Fractal::Fractal(Fractal&& f)
    : mainLine(f.mainLine),
      baseLines(f.baseLines),
      directionLines(f.directionLines),
      lines(f.lines),
      lastReflectionLines(f.lastReflectionLines),
      vertices(f.vertices),
      indices(f.indices),
      depth(f.depth),
      baseLinesCount(f.baseLinesCount),
      directionLinesCount(f.directionLinesCount),
      linesCount(f.linesCount),
      lastReflectionLinesCount(f.lastReflectionLinesCount)
{
  for (size_t i = 0; i < f.linesOnTheLayer.size(); i++) {
    this->linesOnTheLayer.push_back(f.linesOnTheLayer[i]);
  }
  f.baseLines = nullptr;
  f.directionLines = nullptr;
  f.lines = nullptr;
  f.lastReflectionLines = nullptr;
  f.vertices = nullptr;
  f.indices  = nullptr;
}


Fractal::~Fractal() {
  delete[] Fractal::baseLines;
  delete[] Fractal::directionLines;
  delete[] Fractal::lines;
  delete[] Fractal::lastReflectionLines;
  delete[] Fractal::vertices;
  delete[] Fractal::indices;
}


int Fractal::GetLinesCount(const int baseLinesSize, const int directionLinesSize, const int depth) {
  int linesCount = 0;
  for (int i = 1; i <= depth; i++) {
    linesCount += std::pow(directionLinesSize, i - 1);
  }
  linesCount *= baseLinesSize;
  return linesCount;
}
