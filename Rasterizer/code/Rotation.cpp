#include <iomanip>
#include "Rotation.h"

Rotation::Rotation() {
    this->rotationId = -1;
    this->angle = 0;
    this->ux = 0;
    this->uy = 0;
    this->uz = 0;
}

Rotation::Rotation(int rotationId, double angle, double x, double y, double z)
{
    this->rotationId = rotationId;
    this->angle = angle;
    this->ux = x;
    this->uy = y;
    this->uz = z;
}

std::ostream &operator<<(std::ostream &os, const Rotation &r)
{
    os << std::fixed << std::setprecision(3) << "Rotation " << r.rotationId << " => [angle=" << r.angle << ", " << r.ux << ", " << r.uy << ", " << r.uz << "]";
    return os;
}

Matrix4 Rotation::getMatrix()
{
    Vec3 u = *(new Vec3(ux, uy, uz));
    Vec3 v = findV(u);
    Vec3 w = normalizeVec3(crossProductVec3(u, v));

    double matrixM[4][4] = {
        {u.x, u.y, u.z, 0.0},
        {v.x, v.y, v.z, 0.0},
        {w.x, w.y, w.z, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    double matrixMinverse[4][4] = {
        {u.x, v.x, w.x, 0.0},
        {u.y, v.y, w.y, 0.0},
        {u.z, v.z, w.z, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    Matrix4 M = *(new Matrix4(matrixM));
    Matrix4 Minverse = *(new Matrix4(matrixMinverse));

    double radian = angle*3.14159/180;
    double s = sin(radian);
    double c = cos(radian);

    double matrixR[4][4] = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, c, -s, 0.0},
        {0.0, s, c, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    Matrix4 R = *(new Matrix4(matrixR));

    return multiplyMatrixWithMatrix(Minverse, multiplyMatrixWithMatrix(R, M));
}