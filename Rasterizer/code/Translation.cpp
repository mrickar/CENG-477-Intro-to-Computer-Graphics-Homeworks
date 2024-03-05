#include <iomanip>
#include "Translation.h"

Translation::Translation()
{
    this->translationId = -1;
    this->tx = 0.0;
    this->ty = 0.0;
    this->tz = 0.0;
}

Translation::Translation(int translationId, double tx, double ty, double tz)
{
    this->translationId = translationId;
    this->tx = tx;
    this->ty = ty;
    this->tz = tz;
}

std::ostream &operator<<(std::ostream &os, const Translation &t)
{
    os << std::fixed << std::setprecision(3) << "Translation " << t.translationId << " => [" << t.tx << ", " << t.ty << ", " << t.tz << "]";
    return os;
}

Matrix4 Translation::getMatrix()
{
    double matrix_[4][4] = {
        {1.0, 0.0, 0.0, tx},
        {0.0, 1.0, 0.0, ty},
        {0.0, 0.0, 1.0, tz},
        {0.0, 0.0, 0.0, 1.0}
    };

    Matrix4 *matrix = new Matrix4(matrix_);

    return *matrix;
}