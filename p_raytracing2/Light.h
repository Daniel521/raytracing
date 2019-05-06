#pragma once
#include <cassert>
#include "Vector.h"
#include "HDR_RGB.h"

namespace RT {

	class Light {
	public:
		Light() = delete;
		Light(const Light&) = default;
		Light(Light&&) = default;
		Light& operator=(const Light&) = default;
		Light(const Point& location, const HDR_rgb& color, double intensity)
			: location_(location), color_(color), intensity_(intensity) {
			assert(intensity > 0.0);
		}

		const Point& location() const { return location_; }
		const HDR_rgb& color() const { return color_; }
		double intensity() const { return intensity_; }

	private:
		Point location_;
		HDR_rgb color_;
		double intensity_;
	};

}