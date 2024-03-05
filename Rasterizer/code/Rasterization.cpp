#include "Rasterization.h"

// Line rasterization

double interpolated_depth(double x, double y, Vec4 v0, Vec4 v1)
{
    double dist2v0 = std::sqrt((v0.x - x) * (v0.x - x) + (v0.y - y) * (v0.y - y));
    double dist2v1 = std::sqrt((v1.x - x) * (v1.x - x) + (v1.y - y) * (v1.y - y));

    double total = dist2v0 + dist2v1;

    dist2v0 = dist2v0 / total;
    dist2v1 = dist2v1 / total;

    return v0.z * dist2v1 + v1.z * dist2v0;
}

void RasterizeLine(std::vector<std::vector<Color> > *image, std::vector<std::vector<double> > *depth, Line line, std::vector<Vec4 *> *vertices, std::vector<Color *> *colorsOfVertices)
{
    Vec4 v0 = *(*vertices)[line.vertexIds[0] - 1];
    Vec4 v1 = *(*vertices)[line.vertexIds[1] - 1];
    Color c0 = *(*colorsOfVertices)[v0.colorId - 1];
    Color c1 = *(*colorsOfVertices)[v1.colorId - 1];
    
    double slope = (v1.y - v0.y) / (v1.x - v0.x);
    double depth_of_point;

    int x, y;
    double d;
    double cr, cg, cb;
    double dcr, dcg, dcb;

    Vec4 start_v, end_v;
    Color start_c, end_c;
    int verRes = image->size();
    int horRes = (*image)[0].size();

    if (slope <= 1 && slope >=0)
    {
        // march on min_x -> max_x, update y+
        if (v0.x < v1.x)
        {
            start_v = *(new Vec4(v0));
            end_v = *(new Vec4(v1));

            start_c = *(new Color(c0));
            end_c = *(new Color(c1));
        }
        else
        {
            start_v = *(new Vec4(v1));
            end_v = *(new Vec4(v0));

            start_c = *(new Color(c1));
            end_c = *(new Color(c0));
        }

        y = round(start_v.y);
        d = (start_v.y - end_v.y) + 0.5 * (end_v.x - start_v.x);

        cr = start_c.r;
        cg = start_c.g;
        cb = start_c.b;

        dcr = (end_c.r - start_c.r) / (end_v.x - start_v.x);
        dcg = (end_c.g - start_c.g) / (end_v.x - start_v.x);
        dcb = (end_c.b - start_c.b) / (end_v.x - start_v.x);

        for (x = round(start_v.x) ; x < end_v.x && x<verRes && x>=0 && y<horRes && y>=0 ; x++)
        {
            depth_of_point = interpolated_depth(x, y, start_v, end_v);

            if (depth_of_point < (*depth)[x][y])
            {
                (*image)[x][y].r = std::round(cr);
                (*image)[x][y].g = std::round(cg);
                (*image)[x][y].b = std::round(cb);

                (*depth)[x][y] = depth_of_point;
            }

            if (d < 0)
            {
                y++;
                d += ((start_v.y - end_v.y) + (end_v.x - start_v.x));
            }
            else
            {
                d += (start_v.y - end_v.y);
            }

            cr += dcr;
            cg += dcg;
            cb += dcb;
        }
    } 
    else if (slope > 1)
    {
        // march on min_y to max_y, update x+
        if (v0.y < v1.y)
        {
            start_v = *(new Vec4(v0));
            end_v = *(new Vec4(v1));

            start_c = *(new Color(c0));
            end_c = *(new Color(c1));
        }
        else
        {
            start_v = *(new Vec4(v1));
            end_v = *(new Vec4(v0));

            start_c = *(new Color(c1));
            end_c = *(new Color(c0));
        }

        x = round(start_v.x);
        d = (end_v.x - start_v.x) + 0.5 * (start_v.y - end_v.y);

        cr = start_c.r;
        cg = start_c.g;
        cb = start_c.b;

        dcr = (end_c.r - start_c.r) / (end_v.y - start_v.y);
        dcg = (end_c.g - start_c.g) / (end_v.y - start_v.y);
        dcb = (end_c.b - start_c.b) / (end_v.y - start_v.y);

        for (y = round(start_v.y) ; y < end_v.y && x<verRes && x>=0 && y<horRes && y>=0; y++)
        {
            depth_of_point = interpolated_depth(x, y, start_v, end_v);

            if (depth_of_point < (*depth)[x][y])
            {
                (*image)[x][y].r = std::round(cr);
                (*image)[x][y].g = std::round(cg);
                (*image)[x][y].b = std::round(cb);

                (*depth)[x][y] = depth_of_point;
            }

            if (d > 0)
            {
                x++;
                d += ((start_v.y - end_v.y) + (end_v.x - start_v.x));
            }
            else
            {
                d += (end_v.x - start_v.x);
            }

            cr += dcr;
            cg += dcg;
            cb += dcb;
        }
    }
    else if (slope < 0 && slope >= -1)
    {
        // march on min_x to max_x, update y-
        if (v0.x < v1.x)
        {
            start_v = *(new Vec4(v0));
            end_v = *(new Vec4(v1));

            start_c = *(new Color(c0));
            end_c = *(new Color(c1));
        }
        else
        {
            start_v = *(new Vec4(v1));
            end_v = *(new Vec4(v0));

            start_c = *(new Color(c1));
            end_c = *(new Color(c0));
        }

        y = round(start_v.y);
        d = (start_v.y - end_v.y) - 0.5 * (end_v.x - start_v.x);

        cr = start_c.r;
        cg = start_c.g;
        cb = start_c.b;

        dcr = (end_c.r - start_c.r) / (end_v.x - start_v.x);
        dcg = (end_c.g - start_c.g) / (end_v.x - start_v.x);
        dcb = (end_c.b - start_c.b) / (end_v.x - start_v.x);

        for (x = round(start_v.x) ; x < end_v.x && x<verRes && x>=0 && y<horRes && y>=0; x++)
        {
            depth_of_point = interpolated_depth(x, y, start_v, end_v);

            if (depth_of_point < (*depth)[x][y])
            {
                (*image)[x][y].r = std::round(cr);
                (*image)[x][y].g = std::round(cg);
                (*image)[x][y].b = std::round(cb);

                (*depth)[x][y] = depth_of_point;
            }

            if (d > 0)
            {
                y -= 1;
                d += ((start_v.y - end_v.y) - (end_v.x - start_v.x));
            }
            else
            {
                d += (start_v.y - end_v.y);
            }

            cr += dcr;
            cg += dcg;
            cb += dcb;
        }
    }
    else if (slope < -1)
    {
        // march on min_y to max_y, update x-
        if (v0.y < v1.y)
        {
            start_v = *(new Vec4(v0));
            end_v = *(new Vec4(v1));

            start_c = *(new Color(c0));
            end_c = *(new Color(c1));
        }
        else
        {
            start_v = *(new Vec4(v1));
            end_v = *(new Vec4(v0));

            start_c = *(new Color(c1));
            end_c = *(new Color(c0));
        }

        x = round(start_v.x);
        d = -1 * (start_v.y - end_v.y) + 0.5 * (end_v.x - start_v.x);

        cr = start_c.r;
        cg = start_c.g;
        cb = start_c.b;

        dcr = (end_c.r - start_c.r) / (end_v.y - start_v.y);
        dcg = (end_c.g - start_c.g) / (end_v.y - start_v.y);
        dcb = (end_c.b - start_c.b) / (end_v.y - start_v.y);

        for (y = round(start_v.y) ; y < end_v.y && x<verRes && x>=0 && y<horRes && y>=0; y++)
        {
            depth_of_point = interpolated_depth(x, y, start_v, end_v);

            if (depth_of_point < (*depth)[x][y])
            {
                (*image)[x][y].r = std::round(cr);
                (*image)[x][y].g = std::round(cg);
                (*image)[x][y].b = std::round(cb);

                (*depth)[x][y] = depth_of_point;
            }

            if (d < 0)
            {
                x -= 1;
                d += (-1 * (start_v.y - end_v.y) + (end_v.x - start_v.x));
            }
            else
            {
                d += (end_v.x - start_v.x);
            }

            cr += dcr;
            cg += dcg;
            cb += dcb;
        }

    }
}


// Triangle rasterization

double f_01(double x, double y, Vec3 v0, Vec3 v1)
{
    double res = 0;

    res += x * (v0.y - v1.y);
    res += y * (v1.x - v0.x);
    res += v0.x * v1.y;
    res -= v0.y * v1.x;

    return res;
}


double f_12(double x, double y, Vec3 v1, Vec3 v2)
{
    double res = 0;

    res += x * (v1.y - v2.y);
    res += y * (v2.x - v1.x);
    res += v1.x * v2.y;
    res -= v1.y * v2.x;

    return res;
}


double f_20(double x, double y, Vec3 v2, Vec3 v0)
{
    double res = 0;

    res += x * (v2.y - v0.y);
    res += y * (v0.x - v2.x);
    res += v2.x * v0.y;
    res -= v2.y * v0.x;

    return res;
}



void RasterizeTriangle(std::vector<std::vector<Color> > *image, std::vector<std::vector<double> > *depth, Triangle triangle, std::vector<Vec4 *> *vertices, std::vector<Color *> *colorsOfVertices)
{
    Vec4 v0_v4 = *(*vertices)[triangle.vertexIds[0] - 1];
    Vec4 v1_v4 = *(*vertices)[triangle.vertexIds[1] - 1];
    Vec4 v2_v4 = *(*vertices)[triangle.vertexIds[2] - 1];

    Vec3 v0 = Vec3(v0_v4.x, v0_v4.y, v0_v4.z, v0_v4.colorId);
    Vec3 v1 = Vec3(v1_v4.x, v1_v4.y, v1_v4.z, v1_v4.colorId);
    Vec3 v2 = Vec3(v2_v4.x, v2_v4.y, v2_v4.z, v2_v4.colorId);

    Color c0 = *(*colorsOfVertices)[v0.colorId - 1];
    Color c1 = *(*colorsOfVertices)[v1.colorId - 1];
    Color c2 = *(*colorsOfVertices)[v2.colorId - 1];

    double x_max = std::round(max(v0.x, v1.x, v2.x));
    double x_min = std::round(min(v0.x, v1.x, v2.x));
    double y_max = std::round(max(v0.y, v1.y, v2.y));
    double y_min = std::round(min(v0.y, v1.y, v2.y));

    double depth_of_point;

    double alpha, beta, gamma;

    for (int y = y_min ; y <= y_max ; y++)
    {
        for (int x = x_min ; x <= x_max ; x++)
        {
            if (x > 0 && y > 0 && x < (*image).size() && y < (*image)[0].size())
            {
                alpha = f_12(x, y, v1, v2) / f_12(v0.x, v0.y, v1, v2);
                beta = f_20(x, y, v2, v0) / f_20(v1.x, v1.y, v2, v0);
                gamma = f_01(x, y, v0, v1) / f_01(v2.x, v2.y, v0, v1);

                depth_of_point = alpha * v0.z + beta * v1.z + gamma * v2.z;

                if (alpha >= 0 && beta >= 0 && gamma >= 0 && depth_of_point < (*depth)[x][y])
                {
                    (*image)[x][y].r = std::round(alpha * c0.r + beta * c1.r + gamma * c2.r);
                    (*image)[x][y].g = std::round(alpha * c0.g + beta * c1.g + gamma * c2.g);
                    (*image)[x][y].b = std::round(alpha * c0.b + beta * c1.b + gamma * c2.b);

                    (*depth)[x][y] = depth_of_point;
                }
            } 
        }
    }
}