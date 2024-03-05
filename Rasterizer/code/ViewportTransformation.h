#ifndef __VIEWPORT_TRANSFORMATION_H__
#define __VIEWPORT_TRANSFORMATION_H__

#include <iostream>
#include "Matrix4.h"
#include "Vec3.h"
#include "Helpers.h"
#include "cmath"
#include <vector>
#include "Camera.h"

Matrix4 getViewportTransformationMatrix(Camera *camera);

#endif