#include "ViewportTransformation.h"

Matrix4 getViewportTransformationMatrix(Camera *camera)
{
    double nx = camera->horRes;
    double ny = camera->verRes;
    double x_min = camera->left;
    double y_min = camera->bottom;

    // double matrixVP[4][4] = {
    //     {nx/2, 0, 0, (nx/2) - (1/2) + x_min},
    //     {0, ny/2, 0, (ny/2) - (1/2) + y_min},
    //     {0, 0, 1/2, 1/2},
    //     {0, 0, 0, 1}
    // };
    double matrixVP[4][4] = {
        {nx/2, 0, 0, (nx-1)/2},
        {0, ny/2, 0, (ny-1)/2},
        {0, 0, 0.5, 0.5},
        {0, 0, 0, 1}
    };


    Matrix4 *VP = new Matrix4(matrixVP);

    return *VP;
}