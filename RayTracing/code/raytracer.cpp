#include <iostream>
#include "parser.h"
#include "ppm.h"
// #include "additional_structures.h"
#include "vector"
#include "ray_tracer_functions.h"

// REMOVE

#include <iostream>

using namespace std;


typedef unsigned char RGB[3];

int main(int argc, char* argv[])
{
    
    // Sample usage for reading an XML scene file
    parser::Scene scene;
    scene.loadFromXml(argv[1]);
    
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

    for (int camera_idx = 0 ; camera_idx < cameras.size() ; camera_idx++){
        parser::Camera camera = cameras[camera_idx];

        int n_x = camera.image_width;
        int n_y = camera.image_height;

        std::string image_name = camera.image_name;

        unsigned char* image = new unsigned char [n_x * n_y * 3];
        int cnt=0;
        for (int j = 0 ; j < n_y ; j++) { //width => i is column index
            for (int i = 0 ; i < n_x ; i++) { //height => j is row index
                int R_idx = 3 * (j * n_x + i);
                int G_idx = R_idx + 1;
                int B_idx = G_idx + 1;

                additional::Ray ray = create_ray_from_camera(camera, i, j);
                parser::Vec3f float_color = compute_color(ray,scene);

                parser::Vec3i color;
                color.x = (int)float_color.x;
                color.y = (int)float_color.y;
                color.z = (int)float_color.z;

                image[R_idx] = std::min(color.x, 255);
                image[G_idx] = std::min(color.y, 255);
                image[B_idx] = std::min(color.z, 255);
            }
            int cur=(100*j)/n_y;
            if(cnt!=cur)
            {
                cnt=cur;
                std::cout<<cnt<<"%\n";
            }
            
        }

        write_ppm(image_name.c_str(), image, n_x, n_y);
    }
    return 0;

}
