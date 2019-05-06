#pragma once
#include <array>
#include <algorithm>
#include "Misc.h"

namespace RT {

	template <typename scalar_type, size_t DIMENSION>
	class Vector {
	public:
		// Using Declarations and Typenames
		using same_type = Vector<scalar_type, DIMENSION>;
		using storage_type = std::array<scalar_type, DIMENSION>;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

	public:
		// Constructor, Assignment Operator, and Destructor
		Vector() {
			data_.fill(scalar_type());
		}
		Vector(const same_type&  v) = default;
		Vector(same_type&& v) = default;
		Vector(scalar_type default_value) { fill(default_value); }
		template <typename input_iterator>
		Vector(input_iterator begin_, input_iterator end_) {
			auto iter = begin_;
			for (size_t i = 0; i < DIMENSION; ++i) 
				data_[i] = (iter != end_) ? *iter++ : scalar_type(); 
		}
		Vector(std::initializer_list<scalar_type> il) : Vector(il.begin(),il.end()) {}
		same_type& operator=(const same_type& v) = default;
		~Vector() = default;

		// Comparators
		bool operator==(const same_type& rhs) const { return std::equal(data_.begin(), data_.end(), rhs.data_.begin()); }
		bool operator!=(const same_type& rhs) const { return !(*this == rhs); }

		// Accessor
		scalar_type& operator[](size_t i)		{ return data_[i]; }
		const scalar_type& operator[](size_t i) const { return data_[i]; }

		// Math Operations
		same_type operator+() const {
			return *this;
		}
		same_type operator-() const {
			same_type negation_;
			for (size_t i = 0; i < DIMENSION; ++i)
				negation_[i] = data_[i] * -1;
			return negation_;
		}
		same_type operator+(const same_type& rhs) const {
			same_type sum_;
			for (size_t i = 0; i < DIMENSION; ++i)
				sum_[i] = data_[i] + rhs.data_[i];
			return sum_;
		}
		same_type operator-(const same_type& rhs) const {
			same_type difference_;
			for (size_t i = 0; i < DIMENSION; ++i)
				difference_[i] = data_[i] - rhs.data_[i];
			return difference_;
		}
		// Vector-scalar product
		same_type operator*(scalar_type rhs) const { 
			same_type product_;
			for (size_t i = 0; i < DIMENSION; ++i)
				product_[i] = data_[i] * rhs;
			return product_;
		}
		friend same_type operator*(scalar_type scalar, const same_type& vec) {
			same_type product_;
			for (size_t i = 0; i < vec.dimension(); ++i)
				product_[i] = vec.data_[i] * scalar;
			return product_;
		}
		// Vector-Vector product (dot product)
		scalar_type operator*(const same_type& rhs) const {
			scalar_type result_ = 0.0;
			for (size_t i = 0; i < DIMENSION; ++i)
				result_ += (data_[i] * rhs.data_[i]);
			return result_;
		}
		friend scalar_type dot(const same_type& vec0, const same_type& vec1) { return vec0 * vec1; }
		// Vector divided by scalar
		same_type operator/(scalar_type rhs) const {
			same_type quotient_;
			for (size_t i = 0; i < DIMENSION; ++i)
				quotient_[i] = data_[i] / rhs;
			return quotient_;
		}
		// Cross product
		same_type cross(const same_type& rhs) const {
			assert(DIMENSION == 3, "Cross product on a vector with DIMENSION != 3.");
			same_type result_;
			result_[0] = ((data_[1] * rhs.data_[2]) - (data_[2] * rhs.data_[1]));
			result_[1] = ((data_[2] * rhs.data_[0]) - (data_[0] * rhs.data_[2]));
			result_[2] = ((data_[0] * rhs.data_[1]) - (data_[1] * rhs.data_[0]));
			return result_;
		}
		friend same_type cross(const same_type& lhs, const same_type& rhs) { return lhs.cross(rhs); }

		// ostream
		friend std::ostream& operator<<(std::ostream& out, const same_type& vec) {
			out << '<';
			if constexpr (DIMENSION > 0)
				out << vec.data_[0];
			for (size_t i = 1; i < DIMENSION; ++i)
				out << ',' << vec.data_[i];
			out << '>';
			return out;
		}

		// Approximately Equal
		bool approx_equal(const same_type& vec, scalar_type delta) const {
			for (size_t i = 0; i < DIMENSION; ++i) {
				if (approx_equal(data_[i],vec.data_[i], delta) == false) 
					return false;
			}
			return true;
		}

		// Iterators
		iterator begin() { return data_.begin(); }
		iterator end()	 { return data_.end(); }
		const_iterator begin() const { return data_.cbegin(); }
		const_iterator end()   const { return data_.cend(); }

		// Size and indices
		size_t dimension() const { return DIMENSION; }
		bool is_index(size_t i) const { return i < DIMENSION; }

		// Magnitude and Unit vector
		scalar_type magnitude_squared() const {
			scalar_type temp = 0;
			for (size_t i = 0; i < DIMENSION; ++i)
				temp += data_[i]*data_[i];
			return temp;
		}
		scalar_type magnitude() const { return sqrt(magnitude_squared()); }
		same_type normalized() const { return *this / magnitude(); }

		// Misc.
		void fill(scalar_type value) { data_.fill(value); }
	private:
		storage_type data_;
	};

	// Some type aliases for Vector
	template <typename scalar_type> using Vector2 = Vector<scalar_type, 2>;
	template <typename scalar_type> using Vector3 = Vector<scalar_type, 3>;
	template <typename scalar_type> using Vector4 = Vector<scalar_type, 4>;
	using Point = Vector3<double>;
	using Direction = Vector3<double>;

}
