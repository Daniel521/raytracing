#pragma once
#include "Vector.h"
#include <iostream>

namespace RT {

	class Ray {
	public:
		Ray() = delete;
		Ray(const Ray& r) = default;
		Ray& operator=(const Ray& r) = default;
		Ray(const Point& origin, const Direction& direction) : origin_(origin), direction_(direction.normalized()) {}

		const Point& origin() const { return origin_; }
		const Direction& direction() const { return direction_; }

		Point point_along_ray(double t) const { return origin_ + direction_*t; }

		friend std::ostream& operator<<(std::ostream& out, const Ray& ray) {
			return out << "origin=" << ray.origin() << " direction=" << ray.direction();
		}

	private:
		Point origin_;
		Direction direction_;
	};


}