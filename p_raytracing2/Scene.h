#pragma once
#include <cassert>
#include <vector>
#include "Camera.h"
#include "Viewport.h"
#include "Projection.h"
#include "HDR_RGB.h"
#include "Abstract_Object.h"
#include "Abstract_Shader.h"
#include "Mesh.h"
#include "Light.h"

namespace RT {

	class Scene {
	public:
		using object_storage_type = std::vector<Abstract_Object*>;
		using light_storage_type = std::vector<Light*>;

	public:
		Scene(const Scene& s) = default;
		Scene(Scene&& s) = default;
		Scene& operator=(const Scene& s) = default;
		Scene& operator=(Scene&& s) = default;
		Scene() : camera_(nullptr), viewport_(nullptr), projection_(nullptr), background_(BLACK) {}
		Scene(Camera *camera, Viewport *viewport, Abstract_Projection *projection, Abstract_Shader* shader, const HDR_rgb& background)
			: camera_(camera), viewport_(viewport), projection_(projection), shader_(shader), background_(background) {
			assert(camera_); assert(viewport_); assert(projection_); assert(shader_);
			assert(complete());
		}

		bool complete() const { return (camera_ && viewport_ && projection_); }

		const Camera&				camera()	 const { assert(camera_);	  return *camera_; }
		const Viewport&				viewport()	 const { assert(viewport_);   return *viewport_; }
		const Abstract_Projection&	projection() const { assert(projection_); return *projection_; }
		const Abstract_Shader&		shader()	 const { assert(shader_);	  return *shader_; }
		const object_storage_type& objects() const { return objects_; }
		size_t object_count() const { return objects_.size(); }
		const light_storage_type& lights() const { return lights_; }
		const Light& light(size_t i) const { return *lights_[i]; }


		void camera(Camera* cam) { camera_ = cam; }
		void viewport(Viewport* view) { viewport_ = view; }
		void projection(Abstract_Projection* proj) { projection_ = proj; }
		void shader(Abstract_Shader* shade) { shader_ = shade; }
		void background(const HDR_rgb& backg) { background_ = backg; }


		void add_light(Light* lig) { lights_.push_back(lig); }
		void add_object(Abstract_Object* obj) { objects_.push_back(obj); }
		void add_object(Mesh* obj) {
			for (auto& i : *obj)
				objects_.push_back(i);
		}

	
		std::optional<Intersection> intersect(const Ray& ray, double t_min = 0.0, double t_max = DOUBLE_INFINITY) const {
			std::optional<Intersection> best = std::nullopt;
			std::optional<Intersection> temp = std::nullopt;
			for (const auto& i : objects_) {
				temp = i->intersect(ray, t_min, t_max);
				if (temp != std::nullopt) {
					if (temp->t() < t_max) {
						best = temp;
						t_max = temp->t();
					}
				}
			}
			return best;
		}

	private:
		Camera *camera_;
		Viewport *viewport_;
		Abstract_Projection *projection_;
		Abstract_Shader *shader_;
		HDR_rgb background_;
		object_storage_type objects_;
		light_storage_type lights_;
	};

}
