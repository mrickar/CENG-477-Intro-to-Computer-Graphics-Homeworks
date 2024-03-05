#ifndef __ray_tracer_functions_h__
#define __ray_tracer_functions_h__

#include "additional_structures.h"
#include <iostream>
#include <vector>
#include <cmath>

additional::Ray create_ray_from_camera(parser::Camera cam,int j,int i);

additional::Ray create_ray_from_mirror(parser::Vec3f origin, parser::Vec3f w_0);

float ray_intersects_with_sphere(additional::Ray ray, parser::Sphere sphere, std::vector<parser::Vec3f> &vertex_data);

float ray_intersects_with_triangle(additional::Ray ray, parser::Triangle triangle,std::vector<parser::Vec3f> &vertex_data);

additional::T_and_Face_id ray_intersects_with_mesh(additional::Ray ray, parser::Mesh mesh, std::vector<parser::Vec3f> &vertex_data);

parser::Vec3f compute_color(additional::Ray ray,parser::Scene &scene);

parser::Vec3f apply_shading(additional::Ray ray, additional::HitRecord &hitRecord,parser::Scene &scene);

bool not_in_shadow(additional::HitRecord hitRecord, parser::PointLight light, parser::Scene &scene);

parser::Vec3f diffuse_term(additional::HitRecord hitRecord, parser::PointLight light);

parser::Vec3f specular_term(additional::HitRecord hitRecord, parser::PointLight light);

parser::Vec3i compute_L_diffuse(int i, int j, float reflectance, parser::Vec3f w_0, parser::Vec3f w_i);

parser::Vec3i compute_L_ambient(int i, int j, float reflectance, parser::Vec3f w_0, parser::Vec3f w_i);

parser::Vec3i compute_L_specular(int i, int j, float reflectance, float phong, parser::Vec3f w_0, parser::Vec3f w_i);

parser::Vec3i compute_L_mirror(int i, int j, float reflectance, parser::Vec3f w_0, parser::Vec3f w_i);

parser::Vec3f cross_product(parser::Vec3f vec_1,parser::Vec3f vec_2);

parser::Vec3f scaler_vector_multiplication(float multiplier, parser::Vec3f vec);

parser::Vec4f scaler_vector_multiplication(float multiplier, parser::Vec4f vec);

parser::Vec3f scaler_vector_multiplication(parser::Vec3f vec_0, parser::Vec3f vec_1);

parser::Vec3f vector_sum(parser::Vec3f vec_1, parser::Vec3f vec_2);

parser::Vec4f vector_sum(parser::Vec4f vec_1, parser::Vec4f vec_2);

float dot_product(parser::Vec3f vec_1, parser::Vec3f vec_2);

float dot_product(parser::Vec4f vec_1, parser::Vec4f vec_2);

float calculate_determinant(float matrix[3][3]);

float calculate_determinant(parser::Vec3f column_0, parser::Vec3f column_1, parser::Vec3f column_2);

parser::Vec3f vector_sub(parser::Vec3f vec_1, parser::Vec3f vec_2);

parser::Vec3f vector_between_two_points(parser::Vec3f point_1, parser::Vec3f point_2);

float len_of_vec(parser::Vec3f vec);

parser::Vec3f get_sphere_normal(parser::Sphere sphere, parser::Vec3f intersection_point, std::vector<parser::Vec3f> &vertex_data);

parser::Vec3f get_face_normal(parser::Face face, std::vector<parser::Vec3f> &vertex_data);

parser::Vec3f normalize_vec(parser::Vec3f vector);

#endif // __ray_traces_functions_h__