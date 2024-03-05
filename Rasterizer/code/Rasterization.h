#ifndef __RASTERIZATION__
#define __RASTERIZATION__

#include <iostream>
#include "Matrix4.h"
#include "Vec3.h"
#include "Helpers.h"
#include "cmath"
#include <vector>
#include "Camera.h"
#include "Color.h"
#include "Triangle.h"
#include "Line.h"

// Line rasterization

double interpolated_depth(double x, double y, Vec4 v0, Vec4 v1);

void RasterizeLine(std::vector<std::vector<Color> > *image, std::vector<std::vector<double> > *depth, Line line, std::vector<Vec4 *> *vertices, std::vector<Color *> *colorsOfVertices);

// Triangle rasterization
double f_01(double x, double y, Vec3 v0, Vec3 v1);
double f_12(double x, double y, Vec3 v1, Vec3 v2);
double f_20(double x, double y, Vec3 v2, Vec3 v0);

void RasterizeTriangle(std::vector<std::vector<Color> > *image, std::vector<std::vector<double> > *depth, Triangle triangle, std::vector<Vec4 *> *vertices, std::vector<Color *> *colorsOfVertices);

#endif