#pragma once
#include "Vector.h"

namespace RT {

	class Abstract_Object;

	class Intersection {
	public:
		Intersection() = delete;
		Intersection(const Intersection& intersection) = default;
		Intersection& operator=(const Intersection& intersection) = default;
		Intersection(const Abstract_Object* object, const Point& location, double t, const Direction& normal)
			: object_(object), location_(location), t_(t), normal_(normal.normalized()) {
			assert(object != nullptr);
			assert(approx_equal(normal.magnitude(), 1.0, 0.1));
		}

		const Abstract_Object& object() const { return *object_; }
		const Point& location() const { return location_; }
		const Direction& normal() const { return normal_; }
		const double t() const { return t_; }

	private:
		const Abstract_Object* object_;
		Point location_;
		double t_;
		Direction normal_;
	};

}