#include <iostream>
#include <fstream>
#include <optional>
#include "RT.h"
//#define ORTHO_PROJ

using namespace RT;


size_t x_res = 800;
size_t y_res = 800;
Camera camera(Point({ -40.0, 30.0, 20.0 }), Direction({ 1.0, -0.2, -0.5 }), Direction({ 0.0, 1.0, 0.0 }));
Viewport viewport(x_res, y_res, -1, 1, -1, 1);
#ifdef ORTHO_PROJ
Orthographic_Projection projection;
#else
Perspective_Projection projection(1);
#endif
Blinn_Phong_Shader shader(0.1, HDR_rgb(1.0,1.0,1.0), 0.3, 0.3);
//Light light(Point({ 0.0, 8, 0.0 }), HDR_rgb(1.0, 1.0, 1.0), 3);
Light light1(Point({ -50, 400, -200 }), HDR_rgb(1.0, 1.0, 1.0), 20);
Sphere_Object sphere0(Point({ -0.7, 0.0, -2.0 }), 0.5, HDR_rgb(1.0, 0.0, 0.0), 20);
Sphere_Object sphere1(Point({  0.7, 0.0, -2.0 }), 0.8, HDR_rgb(0.0, 1.0, 0.0), 20);

Mesh mesh("slong.obj", HDR_rgb(0.8, 0.9, 0.4), 8);
HDR_rgb background(0.0, 0.0, 0.0);
Scene scene(&camera, &viewport, &projection, &shader, background);

int main() {
	//scene.add_object(&sphere0);
	//scene.add_object(&sphere1);
	scene.add_object(&mesh);
	//scene.add_light(&light);
	scene.add_light(&light1);

	Image image(x_res, y_res);

	std::cout << "Camera: " << camera << std::endl;
	std::cout << "Viewport: " << viewport << std::endl;
	std::cout << "Projection: " << projection << std::endl;
	std::cout << "Background Color: " << background << std::endl;

	for (size_t y = 0; y < image.y_resolution(); ++y) {
		for (size_t x = 0; x < image.x_resolution(); ++x) {
			Vector2<double> uv = scene.viewport().uv(x, y);
			Ray ray = scene.projection().compute_ray(camera, uv[0], uv[1]);
			std::optional<Intersection> intersect = scene.intersect(ray, 0.01, DOUBLE_INFINITY);
			if (intersect == std::nullopt) {
				image.pixel(x, y) = background;
			}
			else {
				image.pixel(x, y) = scene.shader().shade(scene,camera,*intersect);
			}
		}
	}

	ppm_writer(image, "image.ppm");

	return 0;
}
