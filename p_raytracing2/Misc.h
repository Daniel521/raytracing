#pragma once

#include <cassert>
#include <cmath>
#include <limits>

namespace RT {

	const double DOUBLE_INFINITY = std::numeric_limits<double>::infinity();
	const double DOUBLE_NEGATIVE_INFINITY = -DOUBLE_INFINITY;

	template <typename number_type>
	bool approx_equal(number_type a, number_type b, number_type delta) {
		static_assert(!std::numeric_limits<number_type>::is_integer,
			"approx_equal is only defined for floating point types");
		assert(std::isfinite(delta));
		assert(delta > 0);
		return (std::isfinite(a) && std::isfinite(b) &&
			(std::abs(a - b) <= delta));

	}

}