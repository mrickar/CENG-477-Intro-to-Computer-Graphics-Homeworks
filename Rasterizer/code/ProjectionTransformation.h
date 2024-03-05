#ifndef __PROJECTION_TRANSFORMATION_H__
#define __PROJECTION_TRANSFORMATION_H__

#include <iostream>
#include "Matrix4.h"
#include "Vec3.h"
#include "Helpers.h"
#include "cmath"
#include <vector>
#include "Camera.h"

Matrix4 getProjectTrasformationMatrix(Camera *camera);
Vec4* perspectiveDivision(Vec4 *v);

#endif