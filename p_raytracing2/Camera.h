#pragma once
#include <iostream>
#include "Vector.h"
#include "Misc.h"

namespace RT {

	class Camera {
	public:
		Camera() = delete;
		Camera(const Camera& cam) = default;
		Camera& operator=(const Camera& cam) = default;
		Camera(const Point& origin, const Direction& u, const Direction& v, const Direction& w) : origin_(origin), u_(u), v_(v), w_(w) {
			assert(approx_equal(u_.magnitude(), 1.0, 0.1));
			assert(approx_equal(v_.magnitude(), 1.0, 0.1));
			assert(approx_equal(w_.magnitude(), 1.0, 0.1));
		}
		Camera(const Point& origin, const Direction& view_direction, const Direction& up) : origin_(origin) {
			w_ = (-view_direction).normalized();
			u_ = (cross(up, w_)).normalized();
			v_ = (cross(w_, u_)).normalized();
		}

		const Point& origin() const { return origin_; }
		const Direction& u() const { return u_; }
		const Direction& v() const { return v_; }
		const Direction& w() const { return w_; }

		friend std::ostream& operator<<(std::ostream& out, const Camera& camera) {
			return (out << "eye=" << camera.origin() << " u=" << camera.u() << " v=" << camera.v() << " w=" << camera.w());
		}

	private:
		Point origin_;
		Direction u_, v_, w_;
	};

}