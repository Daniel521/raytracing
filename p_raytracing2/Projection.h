#pragma once
#include "Camera.h"
#include "Ray.h"

namespace RT {

	class Abstract_Projection {
	public:
		virtual Ray compute_ray(const Camera& camera, double u, double v) const = 0;
	};

	class Perspective_Projection : public Abstract_Projection {
	public:
		Perspective_Projection() = delete;
		Perspective_Projection(const Perspective_Projection& persp) = default;
		Perspective_Projection(double focal_length) : focal_length_(focal_length) { assert(focal_length_ > 0.0); }

		Ray compute_ray(const Camera& camera, double u, double v) const {
			Point origin		= camera.origin();
			Direction direction = (camera.u()*u + camera.v()*v) - (camera.w()*focal_length_);
			return Ray(origin, direction);
		}
		double focal_length() const { return focal_length_; }

		friend std::ostream& operator<<(std::ostream& out, const Perspective_Projection& p_p) {
			return out << "perspective, focal_length=1";
		}
	private:
		double focal_length_;
	};

	class Orthographic_Projection : public Abstract_Projection {
	public:
		Orthographic_Projection() = default;

		Ray compute_ray(const Camera& camera, double u, double v) const {
			Point origin		= camera.origin() + camera.u()*u + camera.v()*v;
			Direction direction = -camera.w();
			return Ray(origin, direction);
		}
		friend std::ostream& operator<<(std::ostream& out, const Orthographic_Projection& o_p) {
			return out << "orthographic";
		}
	};

}