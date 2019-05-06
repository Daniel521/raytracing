#pragma once
#include <optional>
#include "Abstract_Object.h"
#include "Matrix.h"
#include "Intersection.h"

namespace RT {

	class Triangle_Object : public Abstract_Object {
	public:
		Triangle_Object() = delete;
		Triangle_Object(Point a, Point b, Point c, const HDR_rgb& color, double shininess = 0.1) : a_(a), b_(b), c_(c), Abstract_Object(color,shininess) {}

		const Point& a() const { return a_; }
		const Point& b() const { return b_; }
		const Point& c() const { return c_; }

		virtual std::optional<Intersection> intersect(const Ray& ray, double t_min, double t_max) const {
			assert(t_min < t_max);
			// Setup the matrix, based on the book.
			Matrix3x3<double> mat;
			mat[0][0] = a_[0] - b_[0];
			mat[0][1] = a_[0] - c_[0];
			mat[0][2] = ray.direction()[0];
			mat[1][0] = a_[1] - b_[1];
			mat[1][1] = a_[1] - c_[1];
			mat[1][2] = ray.direction()[1];
			mat[2][0] = a_[2] - b_[2];
			mat[2][1] = a_[2] - c_[2];
			mat[2][2] = ray.direction()[2];
			// Setup the vector
			Vector3<double> vec;
			vec[0] = a_[0] - ray.origin()[0];
			vec[1] = a_[1] - ray.origin()[1];
			vec[2] = a_[2] - ray.origin()[2];
			Vector3<double> byt = mat.solve(vec);

			// Ensure t, beta, and gamma are valid
			if (byt[2] < t_min || byt[2] > t_max)
				return std::nullopt;
			if (byt[1] < 0 || byt[1] > 1)
				return std::nullopt;
			if (byt[0] < 0 || byt[0] > 1 - byt[1])
				return std::nullopt;

			// Calculate intersection and return it
			Vector3<double> point = ray.origin() + ray.direction()*byt[2];
			// Math to find the correct normal
			Vector3<double> normal;
			Vector3<double> normal_candidate_0 = (c_ - a_).cross(b_ - a_).normalized();
			Vector3<double> normal_candidate_1 = (b_ - a_).cross(c_ - a_).normalized();
			auto semi_normal = -ray.direction();
			double temp_0 = normal_candidate_0 * semi_normal;
			double temp_1 = normal_candidate_1 * semi_normal;
			if (abs(temp_0) < abs(temp_1))
				normal = normal_candidate_0;
			else
				normal = normal_candidate_1;
			return std::optional<Intersection>(Intersection(this, point, byt[2], normal));
		}

	private:
		Point a_, b_, c_;
	};

}