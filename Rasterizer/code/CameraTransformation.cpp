#include "CameraTransformation.h"

Matrix4 getCameraTransformationMatrix(Camera *camera) {

    double ex = camera->position.x;
    double ey = camera->position.y;
    double ez = camera->position.z;

    double matrixT[4][4] = {
        {1, 0, 0, -1 * ex},
        {0, 1, 0, -1 * ey},
        {0, 0, 1, -1 * ez},
        {0, 0, 0, 1}
    };

    double matrixR[4][4] = {
        {camera->u.x, camera->u.y, camera->u.z, 0},
        {camera->v.x, camera->v.y, camera->v.z, 0},
        {camera->w.x, camera->w.y, camera->w.z, 0},
        {0, 0, 0, 1}
    };

    Matrix4 *T = new Matrix4(matrixT);
    Matrix4 *R = new Matrix4(matrixR);

    return multiplyMatrixWithMatrix(*R, *T);
}