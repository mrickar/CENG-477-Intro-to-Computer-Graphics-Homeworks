#ifndef __CLIPPING_H__
#define __CLIPPING_H__

#include <iostream>
#include "Matrix4.h"
#include "Vec3.h"
#include "Helpers.h"
#include "cmath"
#include <vector>
#include "Mesh.h"
#include "Camera.h"
#include"Color.h"
// #include"Line.h"
//Liang-Barsky
//Clipping is done in clip space -> after applying projection
void clipTriangles(std::vector<Mesh *> *meshes, std::vector<Vec4 *> *vertices,Camera *camera,std::vector<Color *> *colors);
bool visible(double den,double num,double *t_e,double *t_l);
int getNewColorId(Vec4 *v0,Vec4 *v1,Vec4* newV,std::vector<Color *> *colors);
#endif