#ifndef __ADDITIONAL__STRUCTURES__
#define __ADDITIONAL__STRUCTURES__

#include <string>
#include <vector>
#include "parser.h"

namespace additional {

    struct Ray{
        parser::Vec3f origin;
        parser::Vec3f direction;
        int depth = 0;
    };

    struct ImagePlane{
        float right;
        float left;
        float top;
        float bottom;
        float nx;
        float ny;
    };

    struct HitRecord{
        parser::Material material;
        parser::Vec3f normal;
        parser::Vec3f intersection_point;
        additional::Ray ray;
    };

    struct T_and_Face_id{
        float t;
        int face_id;
    };
}

#endif