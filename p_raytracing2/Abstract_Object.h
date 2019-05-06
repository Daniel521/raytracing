#pragma once
#include <cassert>
#include <optional>
#include "HDR_RGB.h"
#include "Ray.h"

namespace RT {

	class Intersection;

	class Abstract_Object {
	public:
		Abstract_Object() = delete;
		Abstract_Object(const HDR_rgb& color, double shininess = 0.1) : color_(color), shininess_(shininess) {
			assert(shininess_ > 0.0);
		}

		const HDR_rgb& color() const { return color_; }
		double shininess() const { return shininess_; }
		virtual std::optional<Intersection> intersect(const Ray& ray, double t_min, double t_max) const = 0;
		
	private:
		HDR_rgb color_;
		double shininess_;
	};

}