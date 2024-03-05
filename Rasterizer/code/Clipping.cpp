#include "Clipping.h"

void clipTriangles(std::vector<Mesh *> *meshes, std::vector<Vec4 *> *vertices,Camera *camera,std::vector<Color *> *colors){
    
    int mesh_size = meshes->size();
    double r =camera->right;
    double l =camera->left;
    double t =camera->top;
    double b =camera->bottom;
    double n =camera->near;
    double f =camera->far;
    double x_min = -1;
    double x_max = +1;
    double y_min = -1;
    double y_max = +1;
    double z_min = -1;
    double z_max = +1;
    for(int mesh_idx = 0;mesh_idx<mesh_size;mesh_idx++){
        Mesh *cur_mesh = (*meshes)[mesh_idx];
        cur_mesh->lines.clear(); //if there are edges from other cameras
        if (cur_mesh->type == 0) //wireframe
        {
            int number_of_triangles = cur_mesh->numberOfTriangles;
            for(int triangle_idx = 0;triangle_idx<number_of_triangles;triangle_idx++){
                Triangle *cur_triangle = &(cur_mesh->triangles[triangle_idx]);
                std::vector<Line> cur_lines;
                std::vector<std::vector<int>> triangle_edges = {
                    {cur_triangle->vertexIds[0],cur_triangle->vertexIds[1]},
                    {cur_triangle->vertexIds[1],cur_triangle->vertexIds[2]},
                    {cur_triangle->vertexIds[2],cur_triangle->vertexIds[0]},
                };
                
                for(int edge_idx = 0;edge_idx<3;edge_idx++)
                {
                    

                    int v0_id = triangle_edges[edge_idx][0];
                    int v1_id = triangle_edges[edge_idx][1];
                    Vec4* v0 = (*vertices)[v0_id-1];
                    Vec4* v1 = (*vertices)[v1_id-1];

                    double x0 = v0->x;
                    double y0 = v0->y;
                    double z0 = v0->z;
                    
                    double x1 = v1->x;
                    double y1 = v1->y;
                    double z1 = v1->z;
                    
                    double t_e = 0;
                    double t_l = 1;
                    double dx = x1 - x0;
                    double dy = y1 - y0;
                    double dz = z1 - z0;
                    if(visible(dx, x_min-x0, &t_e, &t_l)){ //left
                        if(visible(-dx, x0-x_max, &t_e, &t_l)){ //right
                            if(visible(dy, y_min-y0, &t_e, &t_l)){ //top                               
                                if(visible(-dy, y0-y_max, &t_e, &t_l)){ //bottom     
                                    if(visible(dz, z_min-z0, &t_e, &t_l)){ //front                 
                                        if(visible(-dz, z0-z_max, &t_e, &t_l)){ //back
                                            Line visible_line ;
                                            if(t_e > 0){
                                                x0 = x0 + dx*t_e;
                                                y0 = y0 + dy*t_e;
                                                z0 = z0 + dz*t_e;
                                                Vec4 *newV0 = new Vec4(x0,y0,z0,1);
                                                int color_id = getNewColorId(v0,v1,newV0,colors);
                                                
                                                newV0->colorId = color_id;
                                                (*vertices).push_back(newV0);
                                                visible_line.vertexIds[0] = (*vertices).size(); //Vertices starts from 1
                                                
                                            }
                                            else{
                                                visible_line.vertexIds[0] = v0_id;
                                            }
                                            if(t_l < 1){
                                                x1 = x0 + dx*t_l;
                                                y1 = y0 + dy*t_l;
                                                z1 = z0 + dz*t_l;
                                                Vec4 *newV1 = new Vec4(x1,y1,z1,1);
                                                int color_id = getNewColorId(v0,v1,newV1,colors);
                                                newV1->colorId = color_id;
                                                (*vertices).push_back(newV1);
                                                visible_line.vertexIds[1] = (*vertices).size(); //Vertices starts from 1
                                                
                                            }
                                            else{
                                                visible_line.vertexIds[1] = v1_id;
                                            }
                                            cur_lines.push_back(visible_line);
                                        }
                                    }
                                }
                            }   
                        }
                    }
                }
                cur_mesh->lines.push_back(cur_lines);
            }
        }
    }
}

bool visible(double den,double num,double *t_e,double *t_l){
    double t = num /den;
    if (den > 0){ //potentially entering 
        if(t > *t_l){ 
            return false;
        }
        if(t > *t_e){
            *t_e = t;
        }
    }
    else if (den < 0){ //potentially leaving 
        if(t < *t_e){
            return false;
        }
        if(t < *t_l){
            *t_l = t;
        }
    }
    else if (num > 0){ //parallel
        return false;
    }
    return true;
}

int getNewColorId(Vec4 *v0,Vec4 *v1,Vec4* newV,std::vector<Color *> *colors){
    double total_len = v1->x - v0->x;
    double new_len= newV->x - v0->x;
    double d_r = (*colors)[v1->colorId-1]->r - (*colors)[v0->colorId-1]->r;
    double d_g = (*colors)[v1->colorId-1]->g - (*colors)[v0->colorId-1]->g;
    double d_b = (*colors)[v1->colorId-1]->b - (*colors)[v0->colorId-1]->b;
    double new_r = (*colors)[v0->colorId-1]->r + (new_len * d_r / total_len);
    double new_g = (*colors)[v0->colorId-1]->g + (new_len * d_g / total_len);
    double new_b = (*colors)[v0->colorId-1]->b + (new_len * d_b / total_len);
    Color *new_color = new Color(new_r,new_b,new_g);
    (*colors).push_back(new_color);
    int new_color_id = (*colors).size();
    return new_color_id;
}