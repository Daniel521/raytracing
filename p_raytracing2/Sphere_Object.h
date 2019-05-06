#pragma once
#include <optional>
#include <cassert>
#include "Abstract_Object.h"
#include "Vector.h"
#include "Intersection.h"

namespace RT {

	class Sphere_Object : public Abstract_Object {
	public:
		Sphere_Object() = delete;
		Sphere_Object(const Sphere_Object& sphere) = default;
		Sphere_Object(const Point& center, double radius, const HDR_rgb& color, double shininess = 0.1) 
			: center_(center), radius_(radius), Abstract_Object(color,shininess)
			{ assert(radius_ > 0.0); }

		const Point& center() const { return center_; }
		double radius() const { return radius_; }

		virtual std::optional<Intersection> intersect(const Ray& ray, double t_min, double t_max) const {
			assert(t_min < t_max);
			Vector3<double> center_to_origin = ray.origin() - center_;
			float a = dot(ray.direction(), ray.direction());
			float b = dot(center_to_origin, ray.direction());
			float c = dot(center_to_origin, center_to_origin) - radius_*radius_;
			float discriminant = b*b - a*c;
			if (discriminant > 0.0) {
				float t = (-b - sqrt(discriminant))/a;
				if (t_min <= t && t <= t_max) {
					Point location = ray.point_along_ray(t);
					Direction normal = (location - center_) / radius_;
					return Intersection(this, location, t, normal);
				}
				t = (-b + sqrt(discriminant))/a;
				if (t_min <= t && t <= t_max) {
					Point location = ray.point_along_ray(t);
					Direction normal = (location - center_) / radius_;
					return Intersection(this, location, t, normal);
				}
			}
			return std::nullopt;
		}

		friend std::ostream& operator<<(std::ostream& out, const Sphere_Object& s_o) {
			return out << "center=" << s_o.center() << " radius=" << s_o.radius() << " color=" << s_o.color();
		}

	private:
		Point center_;
		double radius_;
	};

}