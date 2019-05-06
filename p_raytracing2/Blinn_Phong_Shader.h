#pragma once
#include <cassert>
#include "Abstract_Shader.h"
#include "HDR_RGB.h"
#include "Intersection.h"
#include "Camera.h"

namespace RT {

	class Blinn_Phong_Shader : public Abstract_Shader {
	public:
		Blinn_Phong_Shader() = delete;
		Blinn_Phong_Shader(const Blinn_Phong_Shader&) = default;
		Blinn_Phong_Shader(Blinn_Phong_Shader&&) = default;
		Blinn_Phong_Shader(double ambient_coefficient, const HDR_rgb& ambient_color, double diffuse_coefficient, double specular_coefficient)
			: ambient_coefficient_(ambient_coefficient), ambient_color_(ambient_color), 
			diffuse_coefficient_(diffuse_coefficient), specular_coefficient_(specular_coefficient) {
			assert(ambient_coefficient  >= 0.0);
			assert(diffuse_coefficient  >= 0.0);
			assert(specular_coefficient >= 0.0);
		}

		double ambient_coefficient() const { return ambient_coefficient_; }
		const HDR_rgb& ambient_color() const { return ambient_color_; }
		double diffuse_coefficient() const { return diffuse_coefficient_; }
		double specular_coefficient() const { return specular_coefficient_; }

		HDR_rgb shade(const Scene& scene, const Camera& camera, const Intersection& intersection) const {
			// Ambient
			HDR_rgb result
			   (intersection.object().color().r()*ambient_color_.r()*ambient_coefficient_,
				intersection.object().color().g()*ambient_color_.g()*ambient_coefficient_,
				intersection.object().color().b()*ambient_color_.b()*ambient_coefficient_);
			for (size_t i = 0; i < scene.lights().size(); ++i) {
				const Light& light = scene.light(i);
				// Shadow
				Direction direction_ = (light.location() - intersection.location()).normalized();
				Ray  ray(intersection.location(), direction_);
				std::optional<Intersection> inter = scene.intersect(ray, 0.01);
				if (inter != std::nullopt) {
					auto dist_0 = (light.location() -  intersection.location()).magnitude_squared();
					auto dist_1 = (inter->location() - intersection.location()).magnitude_squared();
					if (dist_0 > dist_1) {
						continue;
					}
				}
				// Lambertian
				Vector3<double> l = (light.location() - intersection.normal()).normalized();
				double temp = fmax(0, (intersection.normal()*l));
				HDR_rgb lambert
				   (intersection.object().color().r()*temp*diffuse_coefficient_,
					intersection.object().color().g()*temp*diffuse_coefficient_,
					intersection.object().color().b()*temp*diffuse_coefficient_);
				// Blinn
				Vector3<double> v = (camera.origin() - intersection.location()).normalized();
				auto h = (l + v).normalized();
				temp = fmax(0, (intersection.normal()*h));
				temp = pow(temp, intersection.object().shininess());
				HDR_rgb blinn(specular_coefficient_*temp, specular_coefficient_*temp, specular_coefficient_*temp);
				// Add them together
				result.r((result.r() + lambert.r() + blinn.r() >= 1.0) ? 1.0 : (result.r() + lambert.r() + blinn.r()));
				result.g((result.g() + lambert.g() + blinn.g() >= 1.0) ? 1.0 : (result.g() + lambert.g() + blinn.g()));
				result.b((result.b() + lambert.b() + blinn.b() >= 1.0) ? 1.0 : (result.b() + lambert.b() + blinn.b()));
			}
			return result;
		}

	private:
		double ambient_coefficient_;
		HDR_rgb ambient_color_;
		double diffuse_coefficient_;
		double specular_coefficient_;
	};

}