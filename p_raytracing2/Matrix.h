#pragma once
#include "Vector.h"
#include "Misc.h"

namespace RT {

	template <typename scalar_type, size_t HEIGHT, size_t WIDTH>
	class Matrix {
	public:
		// Type aliases
		using same_type = Matrix<scalar_type, HEIGHT, WIDTH>;
		using row_type = Vector<scalar_type, WIDTH>;
		using row_container_type = std::array<row_type, HEIGHT>;
		using row_iterator = typename row_container_type::iterator;
		using const_row_iterator = typename row_container_type::const_iterator;

	public:
		// Constructor and Assignment
		Matrix() : Matrix(scalar_type(0)) {}
		Matrix(const same_type& m) = default;
		Matrix(same_type&& m) = default;
		Matrix(scalar_type default_value) { fill(default_value); }
		template <typename input_iterator>
		Matrix(input_iterator input_begin, input_iterator input_end) {
			input_iterator iter = input_begin;
			for (size_t y = 0; y < HEIGHT; ++y) {
				for (size_t x = 0; x < WIDTH; ++x) {
					data_[y][x] = (iter == input_end) ? scalar_type(0) : *iter++;
				}
			}
		}
		Matrix(std::initializer_list<scalar_type> il) : Matrix(il.begin(),il.end()) {}
		same_type& operator=(const same_type& m) = default;

		// Comparators
		bool operator==(const same_type& rhs) { return std::equal(data_.begin(), data_.end(), rhs.data_.begin()); }
		bool operator!=(const same_type& rhs) { return !(*this == rhs); }

		// Accessor
		const row_type& operator[](size_t i) const { assert(is_row(i)); return data_[i]; }
		row_type& operator[](size_t i) { assert(is_row(i)); return data_[i]; }

		// Math operations
		const same_type& operator+() const { return *this; }
		same_type operator-() const {
			same_type negation_;
			for (size_t y = 0; y < HEIGHT; ++y)
				for (size_t x = 0; x < WIDTH; ++x)
					negation_[y][x] = - 1 * data_[y][x];
			return negation_;
		}
		same_type operator+(const same_type& rhs) const {
			same_type sum_;
			for (size_t y = 0; y < HEIGHT; ++y)
				for (size_t x = 0; x < WIDTH; ++x)
					sum_[y][x] = data_[y][x] + rhs.data_[y][x];
			return sum_;
		}
		same_type operator-(const same_type& rhs) const {
			same_type difference_;
			for (size_t y = 0; y < HEIGHT; ++y)
				for (size_t x = 0; x < WIDTH; ++x)
					difference_[y][x] = data_[y][x] - rhs.data_[y][x];
			return difference_;
		}
		// Matrix-scalar multiplication
		same_type operator*(scalar_type scalar) const {
			same_type product_;
			for (size_t y = 0; y < HEIGHT; ++y)
				for (size_t x = 0; x < WIDTH; ++x)
					product_[y][x] = data_[y][x] * scalar;
			return product_;
		}
		// Matrix-matrix multiplication
		template <size_t RESULT_WIDTH>
		Matrix<scalar_type, HEIGHT, RESULT_WIDTH> operator*(const Matrix<scalar_type, WIDTH, RESULT_WIDTH>& rhs) const {
			Matrix < scalar_type, HEIGHT< RESULT_WIDTH> result_;
			for (size_t y = 0; y < HEIGHT; ++y) // rows
				for (size_t x = 0; x < RESULT_WIDTH; ++x) // result-width
					for (size_t addies = 0; addies < WIDTH; ++addies) // column
						result_[y][x] += data_[y][addies] * rhs[addies][x];
			return result_;
		}
		same_type operator/(scalar_type scalar) const {
			same_type quotient_;
			for (size_t y = 0; y < HEIGHT; ++y)
				for (size_t x = 0; x < WIDTH; ++x)
					quotient_[y][x] = data_[y][x] / scalar;
			return quotient_;
		}
		// Determinant
		scalar_type determinant() const {
			assert(is_square(), "Must have squared matrix to perform determinant");
			assert((WIDTH == 2 || WIDTH == 3), "Must have 2x2 or 3x3 matrix to perform determinant");

			scalar_type det = 0;
			if (WIDTH == 3) {
				scalar_type row0 = data_[0][0] * ((data_[1][1] * data_[2][2]) - (data_[2][1] * data_[1][2]));
				scalar_type row1 = data_[0][1] * ((data_[1][0] * data_[2][2]) - (data_[2][0] * data_[1][2]));
				scalar_type row2 = data_[0][2] * ((data_[1][0] * data_[2][1]) - (data_[2][0] * data_[1][1]));
				det = row0 - row1 + row2;
			}
			else {
				det = (data_[0][0] * data_[1][1]) - (data_[1][0] * data_[0][1]);
			}
			return det;
		}
		// Solving Linear Equations
		Vector<scalar_type, HEIGHT> solve(const Vector<scalar_type, HEIGHT>& b) const {
			assert(is_square(), "Matrix must be square to solve linear equation");
			assert((WIDTH == 2 || WIDTH == 3), "Matrix must be 2x2 or 3x3 to solve linearly");

			Vector<scalar_type, HEIGHT> result;
			// Cramer's rule
			scalar_type det(0);
			if (WIDTH == 3) {
				scalar_type row0 = data_[0][0] * ((data_[1][1] * data_[2][2]) - (data_[2][1] * data_[1][2]));
				scalar_type row1 = data_[0][1] * ((data_[1][0] * data_[2][2]) - (data_[2][0] * data_[1][2]));
				scalar_type row2 = data_[0][2] * ((data_[1][0] * data_[2][1]) - (data_[2][0] * data_[1][1]));
				det = row0 - row1 + row2;
			}
			else {
				det = (data_[0][0] * data_[1][1]) - (data_[1][0] * data_[0][1]);
			}

			if (WIDTH == 3) {
				scalar_type row0;
				scalar_type row1;
				scalar_type row2;
				//x
				row0 = b[0] * ((data_[1][1] * data_[2][2]) - (data_[2][1] * data_[1][2]));
				row1 = data_[0][1] * ((b[1] * data_[2][2]) - (b[2] * data_[1][2]));
				row2 = data_[0][2] * ((b[1] * data_[2][1]) - (b[2] * data_[1][1]));
				result[0] = (row0 - row1 + row2) / det;
				//y
				row0 = data_[0][0] * ((b[1] * data_[2][2]) - (b[2] * data_[1][2]));
				row1 = b[0] * ((data_[1][0] * data_[2][2]) - (data_[2][0] * data_[1][2]));
				row2 = data_[0][2] * ((data_[1][0] * b[2]) - (data_[2][0] * b[1]));
				result[1] = (row0 - row1 + row2) / det;
				//z
				row0 = data_[0][0] * ((data_[1][1] * b[2]) - (data_[2][1] * b[1]));
				row1 = data_[0][1] * ((data_[1][0] * b[2]) - (data_[2][0] * b[1]));
				row2 = b[0] * ((data_[1][0] * data_[2][1]) - (data_[2][0] * data_[1][1]));
				result[2] = (row0 - row1 + row2) / det;
			}
			else {
				result[0] = ((b[0] * data_[1][1]) - (b[1] * data_[0][1])) / det;
				result[1] = ((data_[0][0] * b[1]) - (data_[1][0] * b[0])) / det;
			}
			return result;

		}

		// Converting to other types


		// Approximately equal
		bool approx_equal(const same_type& rhs, double delta) const {
			for (size_t y = 0; y < HEIGHT; ++y)
				for (size_t x = 0; x < WIDTH; ++x)
					if (approx_equal(data_[y][x], rhs[y][x], delta) == false) return false;
			return true;
		}

		// ostream
		friend std::ostream& operator<<(std::ostream& out, same_type matrix) {
			for (const auto& r : matrix.data_) {
				out << '[';
				if (WIDTH > 0)
					out << r[0];
				for (size_t c = 1; c < WIDTH; ++c)
					out << ' ' << r[c];
				out << ']' << std::endl;
			}
			return out;
		}

		// Iterators
		row_iterator begin() { return data_.begin(); }
		row_iterator end()   { return data_.end(); }
		const_row_iterator begin() const { return data_.cbegin(); }
		const_row_iterator end()   const { return data_.cend(); }

		// Size and indices
		size_t width() const { return WIDTH; }
		size_t height() const { return HEIGHT; }
		bool is_column(size_t i) const { return (i < WIDTH); }
		bool is_row(size_t i) const { return (i < HEIGHT); }
		bool is_row_column(size_t r, size_t c) const { return (is_row(r) && is_column(c)); }
		bool is_square() const { return (WIDTH == HEIGHT); }
		template <typename other_scalar_type, size_t OTHER_HEIGHT, size_t OTHER_WIDTH>
		bool is_same_size(const Matrix<other_scalar_type, OTHER_HEIGHT, OTHER_WIDTH>& other) const {
			return (width() == other.width()) && (height() == other.height());
		}

		// Misc
		void fill(scalar_type value) {
			for (auto& i : data_)
				i.fill(value);
		}
		same_type identity() const {
			same_type ident_;
			for (size_t i = 0; i < HEIGHT && i < WIDTH; ++i)
				ident_[i][i] = 1;
			return ident_;
		}
		Matrix<scalar_type, WIDTH, HEIGHT> tranpose() const {
			Matrix<scalar_type, WIDTH, HEIGHT> trans_;
			for (size_t y = 0; y < WIDTH; ++y)
				for (size_t x = 0; x < HEIGHT; ++x)
					trans_[y][x] = data_[x][y];
			return trans_;
		}

	private:
		row_container_type data_;
	};

	// Type aliases
	template <typename scalar_type> using Matrix2x2 = Matrix<scalar_type, 2, 2>;
	template <typename scalar_type> using Matrix3x3 = Matrix<scalar_type, 3, 3>;
	template <typename scalar_type> using Matrix4x4 = Matrix<scalar_type, 4, 4>;


}