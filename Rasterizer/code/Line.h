#ifndef __LINE_H__
#define __LINE_H__

class Line
{
public:
    int vertexIds[2];

    Line();
    Line(int vid1, int vid2);
    Line(const Line &other);
    friend std::ostream &operator<<(std::ostream &os, const Line &t);
};

#endif