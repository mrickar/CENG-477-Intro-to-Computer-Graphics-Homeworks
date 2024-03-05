#include <iomanip>
#include "Scaling.h"

Scaling::Scaling() {
    this->scalingId = -1;
    this->sx = 0;
    this->sy = 0;
    this->sz = 0;
}

Scaling::Scaling(int scalingId, double sx, double sy, double sz)
{
    this->scalingId = scalingId;
    this->sx = sx;
    this->sy = sy;
    this->sz = sz;
}

std::ostream &operator<<(std::ostream &os, const Scaling &s)
{
    os << std::fixed << std::setprecision(3) << "Scaling " << s.scalingId << " => [" << s.sx << ", " << s.sy << ", " << s.sz << "]";

    return os;
}

Matrix4 Scaling::getMatrix()
{
    double matrix_[4][4] = {
        {sx, 0.0, 0.0, 0.0},
        {0.0, sy, 0.0, 0.0},
        {0.0, 0.0, sz, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    Matrix4 *matrix = new Matrix4(matrix_);

    return *matrix;
}
