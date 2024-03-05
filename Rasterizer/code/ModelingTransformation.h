#ifndef __MODELING_TRANSFORMATION_H__
#define __MODELING_TRANSFORMATION_H__

#include <iostream>
#include "Matrix4.h"
#include "Vec3.h"
#include "Helpers.h"
#include "cmath"
#include <vector>
#include "Mesh.h"
#include "Scaling.h"
#include "Translation.h"
#include "Rotation.h"

std::vector<Matrix4 *> getModelingTrasformationMatricies(std::vector<Mesh *> *meshes, std::vector<Scaling *> *scalings, std::vector<Translation *> *translations, std::vector<Rotation *> *rotations);


#endif