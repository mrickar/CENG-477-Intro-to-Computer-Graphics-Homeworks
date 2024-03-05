#include "Culling.h"

bool isBackFace(Vec4 *v0_4,Vec4 *v1_4,Vec4 *v2_4)
{
    Vec3 v0 = Vec3(v0_4->x,v0_4->y,v0_4->z);
    Vec3 v1 = Vec3(v1_4->x,v1_4->y,v1_4->z);
    Vec3 v2 = Vec3(v2_4->x,v2_4->y,v2_4->z);

    Vec3 v0_to_v1 = subtractVec3(v1,v0);
    Vec3 v0_to_v2 = subtractVec3(v2,v0);
    Vec3 n = crossProductVec3(v0_to_v1,v0_to_v2);

    return dotProductVec3(n,v0) < 0;
}