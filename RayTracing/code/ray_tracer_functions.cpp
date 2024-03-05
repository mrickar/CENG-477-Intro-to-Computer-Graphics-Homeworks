#include "ray_tracer_functions.h"



additional::Ray create_ray_from_camera(parser::Camera cam,int i,int j){
    additional::Ray new_ray;
    additional::ImagePlane image;

    parser::Vec3f e_cam_location = cam.position;

    float distance_cam_image = cam.near_distance;

    image.nx = cam.image_width;
    image.ny = cam.image_height;
    image.left = cam.near_plane.x;
    image.right = cam.near_plane.y;
    image.bottom= cam.near_plane.z;
    image.top= cam.near_plane.w;

    
    parser::Vec3f v = cam.up;
    parser::Vec3f w=scaler_vector_multiplication(-1,cam.gaze); // w = - Gaze
  
    //u = v x w
    parser::Vec3f u = cross_product(v,w);

    parser::Vec3f m = vector_sum(e_cam_location, scaler_vector_multiplication(-1 * distance_cam_image, w));
    parser::Vec3f q = vector_sum(m, vector_sum(scaler_vector_multiplication(image.left, u), scaler_vector_multiplication(image.top, v)));

    float s_u = ((i + 0.5) * (image.right - image.left)) / image.nx;
    float s_v = ((j + 0.5) * (image.top - image.bottom)) / image.ny;

    parser::Vec3f s = vector_sum(q, vector_sum(scaler_vector_multiplication(s_u, u), scaler_vector_multiplication(-1 * s_v, v)));
    
    
    new_ray.origin = e_cam_location;
    new_ray.direction = vector_sum(s, scaler_vector_multiplication(-1, e_cam_location));

    return new_ray;
}

additional::Ray create_ray_from_mirror(additional::Ray incoming, additional::HitRecord hitRecord){
    additional::Ray reflection;
    
    parser::Vec3f incom_normalized = scaler_vector_multiplication(-1,normalize_vec(incoming.direction));
    parser::Vec3f normal_normalized=  normalize_vec(hitRecord.normal);
    float dp = dot_product(normal_normalized,incom_normalized);
    parser::Vec3f first_ele =scaler_vector_multiplication(2*dp, normal_normalized);
    parser::Vec3f reflection_dir= vector_sub(first_ele,incom_normalized);

    reflection.direction = reflection_dir;
    reflection.origin=hitRecord.intersection_point;
    reflection.depth = incoming.depth + 1;

    return reflection;
}

float ray_intersects_with_sphere(additional::Ray ray, parser::Sphere sphere, std::vector<parser::Vec3f> &vertex_data) {
    
    int c_vertex_idx = sphere.center_vertex_id - 1; 
    parser::Vec3f c = vertex_data[c_vertex_idx];
    parser::Vec3f o = ray.origin;
    parser::Vec3f d = ray.direction;
    float R = sphere.radius;

    float discriminant;
    float discriminant_left;
    float discriminant_right;

    parser::Vec3f o_minus_c = vector_sum(o, scaler_vector_multiplication(-1, c));

    discriminant_left = dot_product(d, o_minus_c);
    discriminant_left = discriminant_left * discriminant_left;

    discriminant_right = dot_product(o_minus_c, o_minus_c);
    discriminant_right = discriminant_right - (R * R);
    discriminant_right = dot_product(d, d) * discriminant_right;

    discriminant = discriminant_left - discriminant_right;

    if (discriminant <= 0) {
        return -1;
    }
    
    float t_base, t_1, t_2;

    t_base = -1 * dot_product(d, o_minus_c);
    t_1 = (t_base + std::sqrt(discriminant)) / (dot_product(d, d));
    t_2 = (t_base - std::sqrt(discriminant)) / (dot_product(d, d));

    if (t_1 < 0 and t_2 < 0) {
        return -1;
    }
    else if (t_1 < 0) {
        return t_2;
    } else if (t_2 < 0) {
        return t_1;
    } else {
        return std::min(t_1, t_2);
    }


    
}

float ray_intersects_with_triangle(additional::Ray ray, parser::Triangle triangle, std::vector<parser::Vec3f> &vertex_data) {
    parser::Vec3f a=vertex_data[triangle.indices.v0_id-1];
    parser::Vec3f b=vertex_data[triangle.indices.v1_id-1];
    parser::Vec3f c=vertex_data[triangle.indices.v2_id-1];
    
    
    parser::Vec3f a_minus_c= vector_sub(a,c);
    parser::Vec3f b_minus_c= vector_sub(b,c);
    parser::Vec3f minus_d=scaler_vector_multiplication(-1,ray.direction);
    parser::Vec3f e_minus_c= vector_sub(ray.origin,c); //e is origin

    float determinant_A = calculate_determinant(a_minus_c,b_minus_c,minus_d);
    float alpha = calculate_determinant(e_minus_c,b_minus_c,minus_d) / determinant_A;
    float beta = calculate_determinant(a_minus_c,e_minus_c,minus_d) / determinant_A;
    float gamma = 1 - (alpha+beta);
    float t = calculate_determinant(a_minus_c,b_minus_c,e_minus_c) / determinant_A;


    //there is intersection
    if((0<= alpha && alpha<=1) && (0<= beta && beta<=1) && (0<= gamma && gamma<=1) && t>0)
    {
        return t;
    }
    
    return -1;
}

additional::T_and_Face_id ray_intersects_with_mesh(additional::Ray ray, parser::Mesh mesh, std::vector<parser::Vec3f> &vertex_data) {
    std::vector<parser::Face> faces = mesh.faces;
    parser::Triangle triangle;
    triangle.material_id = mesh.material_id;
    float return_t = __FLT_MAX__;
    float current_t = -1;
    int return_face_idx = -1;

    for (int face_idx = 0 ; face_idx < faces.size() ; face_idx++) {
        triangle.indices = mesh.faces[face_idx];
        current_t = ray_intersects_with_triangle(ray, triangle, vertex_data);
        if (current_t != -1 and current_t < return_t) {
            return_t = current_t;
            return_face_idx = face_idx;
        }
    }
    additional::T_and_Face_id return_struct;

    if (return_face_idx != -1) {
        return_struct.t = return_t;
        return_struct.face_id = return_face_idx;
    } else {
        return_struct.t = -1;
        return_struct.face_id = -1;
    }

    return return_struct;
}

parser::Vec3f compute_color(additional::Ray ray,parser::Scene &scene)
{
    parser::Vec3f color;
    if(ray.depth > scene.max_recursion_depth )
    {
        color.x = 0;
        color.y = 0;
        color.z = 0;
        return color;
    }
    parser::Vec3i background_color = scene.background_color;
    float shadow_ray_epsilon = scene.shadow_ray_epsilon;
    int max_recursion_depth = scene.max_recursion_depth;
    std::vector<parser::Camera> cameras = scene.cameras;
    parser::Vec3f ambient_light = scene.ambient_light;
    std::vector<parser::PointLight> point_lights = scene.point_lights;
    std::vector<parser::Material> materials = scene.materials;
    std::vector<parser::Vec3f> vertex_data = scene.vertex_data;
    std::vector<parser::Mesh> meshes = scene.meshes;
    std::vector<parser::Triangle> triangles = scene.triangles;
    std::vector<parser::Sphere> spheres = scene.spheres;
    
    float t_min = __FLT_MAX__;
    std::string object_type;
    int object_idx = -1;
    parser::Material hitRecord_material_type;
    parser::Vec3f hitRecord_intersection_point;
    parser::Vec3f hitRecord_normal;
    additional::Ray hitRecord_ray;

    additional::HitRecord hit_record;
    // check mesh intersections
    for (int mesh_idx = 0 ; mesh_idx < meshes.size() ; mesh_idx++) {
        additional::T_and_Face_id t_and_face = ray_intersects_with_mesh(ray, meshes[mesh_idx], vertex_data);
        float t = t_and_face.t;
        int face_id = t_and_face.face_id;
        if (t != -1 && t < t_min) {
            object_type = "MESH";
            object_idx = mesh_idx;
            t_min = t;
            hitRecord_material_type = materials[meshes[mesh_idx].material_id-1];
            hitRecord_intersection_point = vector_sum(ray.origin, scaler_vector_multiplication(t, ray.direction));
            hitRecord_normal = get_face_normal(meshes[mesh_idx].faces[face_id], vertex_data);
            hitRecord_ray = ray;
        }
    }

    // check triangle intersections
    for (int triangle_idx = 0 ; triangle_idx < triangles.size() ; triangle_idx++) {
        
        float t = ray_intersects_with_triangle(ray, triangles[triangle_idx], vertex_data);
        if (t != -1 && t < t_min) {
            object_type = "TRIANGLE";
            object_idx = triangle_idx;
            t_min = t;
            hitRecord_material_type = materials[triangles[triangle_idx].material_id-1];
            hitRecord_intersection_point = vector_sum(ray.origin, scaler_vector_multiplication(t, ray.direction));
            hitRecord_normal = get_face_normal(triangles[triangle_idx].indices, vertex_data);
            hitRecord_ray = ray;
        }
    }

    // check sphere intersections
    for (int sphere_idx = 0 ; sphere_idx < spheres.size() ; sphere_idx++) {
        float t = ray_intersects_with_sphere(ray, spheres[sphere_idx], vertex_data);
        if (t  != -1 && t < t_min && t > 0) {
            object_type = "SPHERE";
            object_idx =sphere_idx;
            t_min = t;
            hitRecord_material_type = materials[spheres[sphere_idx].material_id-1];
            hitRecord_intersection_point = vector_sum(ray.origin, scaler_vector_multiplication(t, ray.direction));
            hitRecord_normal = get_sphere_normal(spheres[sphere_idx], hitRecord_intersection_point, vertex_data);
            hitRecord_ray = ray;
        }
    }

    if (object_idx != -1) {
        hit_record.material = hitRecord_material_type;
        hit_record.intersection_point = hitRecord_intersection_point;
        hit_record.normal = hitRecord_normal;
        hit_record.ray = hitRecord_ray;
        return apply_shading(ray,hit_record,scene);
    } 
    else if(ray.depth==0){
        color.x = scene.background_color.x;
        color.y = scene.background_color.y;
        color.z = scene.background_color.z;
        return color;
    }   
    else{
        color.x = 0;
        color.y = 0;
        color.z = 0;
        return color;
    }

}

parser::Vec3f apply_shading(additional::Ray ray, additional::HitRecord &hitRecord,parser::Scene &scene){
    parser::Vec3f color;
    color.x = 0;
    color.y = 0;
    color.z = 0;
    
    float shadow_ray_epsilon = scene.shadow_ray_epsilon;
    parser::Vec3f ambient_light = scene.ambient_light;
    std::vector<parser::PointLight> point_lights = scene.point_lights;

    //Ambient light
    color = vector_sum(color, scaler_vector_multiplication(ambient_light,hitRecord.material.ambient));

    if(hitRecord.material.is_mirror)
    {
        additional::Ray reflection_ray =  create_ray_from_mirror(ray, hitRecord);
        reflection_ray.origin =vector_sum(reflection_ray.origin,scaler_vector_multiplication(scene.shadow_ray_epsilon,reflection_ray.direction));
        
        color = vector_sum(color,scaler_vector_multiplication(compute_color(reflection_ray,scene), hitRecord.material.mirror));
    }

    for(int point_light_id=0;point_light_id<point_lights.size();point_light_id++)
    {
        parser::PointLight point_light = point_lights[point_light_id];
        if(not_in_shadow(hitRecord, point_light, scene))
        {
            color = vector_sum(color, vector_sum(diffuse_term(hitRecord,point_light), specular_term(hitRecord,point_light)));
        }
    }
    return color;
}

bool not_in_shadow(additional::HitRecord hitRecord, parser::PointLight light, parser::Scene &scene)
{
    additional::Ray shadow_ray;
    shadow_ray.origin = vector_sum(hitRecord.intersection_point, scaler_vector_multiplication(scene.shadow_ray_epsilon, hitRecord.normal));
    shadow_ray.direction = vector_sub(light.position, hitRecord.intersection_point); 

    // check if it intercests

    bool intersects = false;

    // check mesh intersections
    for (int mesh_idx = 0 ; mesh_idx < scene.meshes.size() ; mesh_idx++) {
        additional::T_and_Face_id t_and_face = ray_intersects_with_mesh(shadow_ray, scene.meshes[mesh_idx], scene.vertex_data);
        float t = t_and_face.t;
        if (t != -1) {
            intersects = true;
            break;
        }
    }

    if (intersects) {
        return false;
    }

    // check triangle intersections
    for (int triangle_idx = 0 ; triangle_idx < scene.triangles.size() ; triangle_idx++) {
        
        float t = ray_intersects_with_triangle(shadow_ray, scene.triangles[triangle_idx], scene.vertex_data);
        if (t != -1) {
            intersects = true;
            break;
        }
    }

    if (intersects) {
        return false;
    }

    // check sphere intersections
    for (int sphere_idx = 0 ; sphere_idx < scene.spheres.size() ; sphere_idx++) {
        float t = ray_intersects_with_sphere(shadow_ray, scene.spheres[sphere_idx], scene.vertex_data);
        if (t  != -1 && t > 0) {
            intersects = true;
            break;
        }
    }

    return !intersects;

}

parser::Vec3f diffuse_term(additional::HitRecord hitRecord, parser::PointLight light)
{
    parser::Vec3f color;
    parser::Vec3f directed_light = vector_sub(light.position, hitRecord.intersection_point);
    float directed_light_len = len_of_vec(directed_light);
    parser::Vec3f irradiance;

    irradiance.x = light.intensity.x / (directed_light_len * directed_light_len);
    irradiance.y = light.intensity.y / (directed_light_len * directed_light_len);
    irradiance.z = light.intensity.z / (directed_light_len * directed_light_len);

    float cos_theta_prime;

    cos_theta_prime = dot_product(normalize_vec(directed_light), hitRecord.normal);
    cos_theta_prime = std::max(0.0f, cos_theta_prime);

    color.x = hitRecord.material.diffuse.x * cos_theta_prime * irradiance.x;
    color.y = hitRecord.material.diffuse.y * cos_theta_prime * irradiance.y;
    color.z = hitRecord.material.diffuse.z * cos_theta_prime * irradiance.z;

    return color;
}

parser::Vec3f specular_term(additional::HitRecord hitRecord, parser::PointLight light)
{
    parser::Vec3f color;
    parser::Vec3f directed_light = vector_sub(light.position, hitRecord.intersection_point);
    float directed_light_len = len_of_vec(directed_light);
    parser::Vec3f negative_ray = scaler_vector_multiplication(-1, hitRecord.ray.direction);
    parser::Vec3f light_negative_ray_sum = vector_sum(normalize_vec(directed_light), negative_ray);
    parser::Vec3f half_vector = scaler_vector_multiplication(1 / len_of_vec(light_negative_ray_sum), light_negative_ray_sum);


    parser::Vec3f irradiance;

    irradiance.x = light.intensity.x / (directed_light_len * directed_light_len);
    irradiance.y = light.intensity.y / (directed_light_len * directed_light_len);
    irradiance.z = light.intensity.z / (directed_light_len * directed_light_len);

    float cos_alpha_prime;

    cos_alpha_prime = dot_product(hitRecord.normal, half_vector);
    cos_alpha_prime = std::max(0.0f, cos_alpha_prime);
    cos_alpha_prime = std::pow(cos_alpha_prime, hitRecord.material.phong_exponent);

    color.x = hitRecord.material.specular.x * cos_alpha_prime * irradiance.x;
    color.y = hitRecord.material.specular.y * cos_alpha_prime * irradiance.y;
    color.z = hitRecord.material.specular.z * cos_alpha_prime * irradiance.z;

    return color;
}

parser::Vec3f cross_product(parser::Vec3f vec_1,parser::Vec3f vec_2)
{

    parser::Vec3f result;
    result.x = vec_1.y * vec_2.z - vec_1.z * vec_2.y;
    result.y = -(vec_1.x * vec_2.z) + vec_1.z * vec_2.x;
    result.z = vec_1.x * vec_2.y - vec_1.y * vec_2.x;
    
    return result;
}

parser::Vec3f scaler_vector_multiplication(float multiplier, parser::Vec3f vec)
{
    parser::Vec3f new_vec;
    new_vec.x = multiplier * vec.x;
    new_vec.y = multiplier * vec.y;
    new_vec.z = multiplier * vec.z;

    return new_vec;
}

parser::Vec3f scaler_vector_multiplication(parser::Vec3f vec_0, parser::Vec3f vec_1)
{
    parser::Vec3f new_vec;
    new_vec.x = vec_0.x * vec_1.x;
    new_vec.y = vec_0.y * vec_1.y;
    new_vec.z = vec_0.z * vec_1.z;

    return new_vec;
}

parser::Vec4f scaler_vector_multiplication(float multiplier, parser::Vec4f vec)
{
    parser::Vec4f new_vec;
    new_vec.x = multiplier * vec.x;
    new_vec.y = multiplier * vec.y;
    new_vec.z = multiplier * vec.z;
    new_vec.w = multiplier * vec.w;

    return new_vec;
}


parser::Vec3f vector_sum(parser::Vec3f vec_1, parser::Vec3f vec_2)
{
    parser::Vec3f new_vec;
    new_vec.x = vec_1.x + vec_2.x;
    new_vec.y = vec_1.y + vec_2.y;
    new_vec.z = vec_1.z + vec_2.z;

    return new_vec;
}

parser::Vec4f vector_sum(parser::Vec4f vec_1, parser::Vec4f vec_2)
{
    parser::Vec4f new_vec;
    new_vec.x = vec_1.x + vec_2.x;
    new_vec.y = vec_1.y + vec_2.y;
    new_vec.z = vec_1.z + vec_2.z;
    new_vec.w = vec_1.w + vec_2.w;

    return new_vec;
}

parser::Vec3f vector_sub(parser::Vec3f vec_1, parser::Vec3f vec_2)
{
    parser::Vec3f new_vec;
    new_vec.x = vec_1.x - vec_2.x;
    new_vec.y = vec_1.y - vec_2.y;
    new_vec.z = vec_1.z - vec_2.z;

    return new_vec;
}

float len_of_vec(parser::Vec3f vec) {
    float len_square = 0;

    len_square += vec.x * vec.x;
    len_square += vec.y * vec.y;
    len_square += vec.z * vec.z;

    return std::sqrt(len_square);
}

float dot_product(parser::Vec3f vec_1, parser::Vec3f vec_2) {
    return vec_1.x * vec_2.x + vec_1.y * vec_2.y + vec_1.z * vec_2.z;
}

float dot_product(parser::Vec4f vec_1, parser::Vec4f vec_2) {
    return vec_1.x * vec_2.x + vec_1.y * vec_2.y + vec_1.z * vec_2.z + vec_1.w * vec_2.w;
}


float calculate_determinant(float matrix[3][3]){

    float result;

    result = matrix[0][0]*(matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) - matrix[0][1]*(matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) + matrix[0][2]*(matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    
    return result;
}

float calculate_determinant(parser::Vec3f column_0, parser::Vec3f column_1, parser::Vec3f column_2){

    return column_0.x * (column_1.y*column_2.z -column_1.z*column_2.y) - column_1.x * (column_0.y*column_2.z - column_0.z*column_2.y) + column_2.x * (column_0.y*column_1.z - column_0.z*column_1.y);
}

parser::Vec3f get_sphere_normal(parser::Sphere sphere, parser::Vec3f intersection_point, std::vector<parser::Vec3f> &vertex_data) {
    parser::Vec3f normal;

    normal = vector_sub(intersection_point, vertex_data[sphere.center_vertex_id - 1]);
    normal = scaler_vector_multiplication(1 / sphere.radius, normal);

    return normal;
}

parser::Vec3f get_face_normal(parser::Face face, std::vector<parser::Vec3f> &vertex_data) {
    parser::Vec3f a = vertex_data[face.v0_id - 1];
    parser::Vec3f b = vertex_data[face.v1_id - 1];
    parser::Vec3f c = vertex_data[face.v2_id - 1];

    parser::Vec3f normal = cross_product(vector_sub(b, a), vector_sub(c, a));

    float len_n = len_of_vec(normal);

    normal = scaler_vector_multiplication(1 / len_n, normal);

    return normal;
}

parser::Vec3f normalize_vec(parser::Vec3f vector) {
    float magnitude = len_of_vec(vector);

    return scaler_vector_multiplication(1 / magnitude, vector);
}