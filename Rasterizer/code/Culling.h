#ifndef __CULLING_H__
#define __CULLING_H__

#include <iostream>
#include "Matrix4.h"
#include "Vec3.h"
#include "Helpers.h"
#include "cmath"
#include <vector>
#include "Camera.h"
#include "Mesh.h"

bool isBackFace(Vec4 *v0,Vec4 *v1,Vec4 *v2);

#endif