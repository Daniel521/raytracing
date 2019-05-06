#pragma once
#include "Vector.h"

namespace RT {

	class Viewport {
	public:
		Viewport() = delete;
		Viewport(const Viewport& viewport) = default;
		Viewport& operator=(const Viewport& viewport) = default;
		Viewport(size_t x_res, size_t y_res, double left, double right, double bottom, double top) 
			: x_resolution_(x_res), y_resolution_(y_res), 
			left_(left), right_(right), bottom_(bottom), top_(top) {
			assert(x_resolution_ > 0);
			assert(y_resolution_ > 0);
			assert(left_ < right_);
			assert(bottom_ < top_);
			assert(left_ < 0.0);
			assert(right_ > 0.0);
			assert(bottom_ < 0.0);
			assert(top_ > 0.0);
		}

		size_t x_resolution() const { return x_resolution_; }
		size_t y_resolution() const { return y_resolution_; }
		double left()   const { return left_; }
		double right()  const { return right_; }
		double bottom() const { return bottom_; }
		double top()	const { return top_; }

		// Converts a coordinate pixel, to (u,v)
			// u : left to right
			// v : bottom to top
		Vector2<double> uv(size_t x, size_t y) const {
			double u = left_ + (right_ - left_)*(x + 0.5)/x_resolution_;
			double v = bottom_ + (top_ - bottom_)*(y + 0.5)/y_resolution_;
			return Vector2<double>({ u,v });
		}

		friend std::ostream& operator<<(std::ostream& out, const Viewport& viewport) {
			out << "x_res=" << viewport.x_resolution() << " y_res=" << viewport.y_resolution()
				<< " left=" << viewport.left() << " right=" << viewport.right()
				<< " bottom=" << viewport.bottom() << " top=" << viewport.top();
			return out;
		}

	private:
		size_t x_resolution_, y_resolution_;
		double left_, right_, bottom_, top_;
	};

}