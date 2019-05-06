#pragma once
#include "HDR_RGB.h"

namespace RT {

	class Image {
	public:
		Image() = delete;
		Image(const Image& image) : x_resolution_(image.x_resolution_), y_resolution_(image.y_resolution_), data_(new HDR_rgb*[y_resolution_]) {
			for (size_t y = 0; y < y_resolution_; ++y) {
				data_[y] = new HDR_rgb[x_resolution_];
				for (size_t x = 0; x < x_resolution_; ++x)
					data_[y][x] = image.data_[y][x];
			}
		}
		Image(size_t x_res, size_t y_res) : x_resolution_(x_res), y_resolution_(y_res), data_(new HDR_rgb*[y_resolution_]) {
			for (size_t y = 0; y < y_resolution_; ++y) {
				data_[y] = new HDR_rgb[x_resolution_];
			}
		}
		~Image() {
			for (size_t y = 0; y < y_resolution_; ++y)
				delete[] data_[y];
			delete[] data_;
		}

		size_t x_resolution() const { return x_resolution_; }
		size_t y_resolution() const { return y_resolution_; }
		HDR_rgb* const operator[](size_t i) { return data_[i]; }
		HDR_rgb pixel(size_t x, size_t y) const { return data_[y][x]; }
		HDR_rgb& pixel(size_t x, size_t y) { return data_[y][x]; }

		friend std::ostream& operator<<(std::ostream& out, const Image& image) {
			for (int y = image.y_resolution() - 1; y >= 0; --y) {
				for (int x = 0; x < image.x_resolution(); ++x) {
					RGB_888 color = image.pixel(x, y).rgb_888();
					out << color.r() << ' ' << color.g() << ' ' << color.b() << std::endl;
				}
			}
			return out;
		}

	private:
		size_t x_resolution_, y_resolution_;
		HDR_rgb** data_;
	};

}