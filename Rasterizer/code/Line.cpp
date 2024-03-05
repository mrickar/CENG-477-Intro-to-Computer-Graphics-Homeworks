#include <iomanip>
#include "Line.h"

Line::Line()
{
    this->vertexIds[0] = -1;
    this->vertexIds[1] = -1;
}

Line::Line(int vid1, int vid2)
{
    this->vertexIds[0] = vid1;
    this->vertexIds[1] = vid2;

}

Line::Line(const Line &other)
{
    this->vertexIds[0] = other.vertexIds[0];
    this->vertexIds[1] = other.vertexIds[1];
}

std::ostream &operator<<(std::ostream &os, const Line &t)
{
    os << std::fixed << std::setprecision(0) << "Line with vertices (" << t.vertexIds[0] << ", " << t.vertexIds[1] <<  ")";
    return os;
}