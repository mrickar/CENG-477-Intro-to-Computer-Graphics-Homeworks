#include "ProjectionTransformation.h"

Matrix4 getProjectTrasformationMatrix(Camera *camera){
    double r = camera->right;
    double l = camera->left;
    double t = camera->top;
    double b = camera->bottom;
    double n = camera->near;
    double f = camera->far;
    if (camera->projectionType==0){ //orthographic
        double values[4][4] = { {(2/(r-l)),0,0,(-(r+l)/(r-l))},
                                {0,(2/(t-b)),0,(-(t+b)/(t-b))},
                                {0,0,(-2/(f-n)),(-(f+n)/(f-n))},
                                {0,0,0,1}};
        return Matrix4(values);
    }
    else if(camera->projectionType==1){ //perspective
        double values[4][4] = { {(2*n/(r-l)),0,((r+l)/(r-l)),0},
                                {0,(2*n/(t-b)),(t+b)/(t-b),0},
                                {0,0,(f+n)/(n-f),(-2*f*n)/(f-n)},
                                {0,0,-1,0}};
        return Matrix4(values);
    } 
}
Vec4* perspectiveDivision(Vec4 *v4){
    Vec4 *v= new Vec4(v4->x/v4->t,v4->y/v4->t,v4->z/v4->t,1,v4->colorId);
    return v;
}
