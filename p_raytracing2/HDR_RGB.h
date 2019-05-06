#pragma once
#include <array>
#include <cassert>
#include <iostream>

namespace RT {

	class RGB_888 {
	public:
		using same_type = RGB_888;
		using storage_type = std::array<uint8_t, 3>;
		enum { R = 0, G = 1, B = 2 };
	public:
		// Constructor, Assignment
		RGB_888(uint8_t arg_r, uint8_t arg_g, uint8_t arg_b) {
			data_[R] = arg_r; data_[G] = arg_g; data_[B] = arg_b;
		}
		RGB_888() : RGB_888(0, 0, 0) {}
		RGB_888(const RGB_888& col) = default;
		RGB_888(RGB_888&& col) = default;
		RGB_888& operator=(const RGB_888& col) = default;

		// Accessor
		int r() const { return static_cast<int>(data_[R]); }
		int g() const { return static_cast<int>(data_[G]); }
		int b() const { return static_cast<int>(data_[B]); }

		friend std::ostream& operator<<(std::ostream& out, const RGB_888& col) {
			out << col.r() << ' ' << col.g() << ' ' << col.b();
			return out;
		}
	private:
		storage_type data_;
	};

	class HDR_rgb {
	public:
		enum { R = 0, G = 1, B = 2};
		using intensity = double;
		using storage_type = std::array<intensity, 3>;
	public:
		static bool is_valid_intensity(intensity r) { return r <= 1.0 ? true : false; }
		static intensity byte_to_intensity(uint8_t byte) {
			return static_cast<intensity>(byte) / 255.0;
		}
		static HDR_rgb bytes_to_HDR_rgb(uint8_t arg_r, uint8_t arg_g, uint8_t arg_b) {
			return HDR_rgb(byte_to_intensity(arg_r), byte_to_intensity(arg_g), byte_to_intensity(arg_b));
		}
		static uint8_t intensity_to_byte(intensity r) {
			return static_cast<uint8_t>(r*255.0);
		}
	public:
		HDR_rgb(intensity r, intensity g, intensity b) {
			assert(is_valid_intensity(r));
			assert(is_valid_intensity(r));
			assert(is_valid_intensity(r));
			data_[R] = r; data_[G] = g; data_[B] = b;
		}
		HDR_rgb() : HDR_rgb(0.0,0.0,0.0) {}
		HDR_rgb(const HDR_rgb& col) = default;
		HDR_rgb(HDR_rgb&& col) = default;
		HDR_rgb& operator=(const HDR_rgb& col) = default;
		~HDR_rgb() = default;

		// Accessors and Manipulators
		intensity r() const { return data_[R]; }
		intensity g() const { return data_[G]; }
		intensity b() const { return data_[B]; }
		void r(intensity arg_r) { 
			assert(is_valid_intensity(arg_r));
			data_[R] = arg_r; 
		}	
		void g(intensity arg_g) {
			assert(is_valid_intensity(arg_g));
			data_[G] = arg_g;
		}
		void b(intensity arg_b) {
			assert(is_valid_intensity(arg_b));
			data_[B] = arg_b;
		}
		void assign(intensity arg_r, intensity arg_g, intensity arg_b) { 
			r(arg_r); g(arg_g); b(arg_b); 
		}
		void fill(intensity val) {
			assert(is_valid_intensity(val));
			data_.fill(val);
		}

		RGB_888 rgb_888() const {
			return RGB_888(intensity_to_byte(r()), intensity_to_byte(g()), intensity_to_byte(b()));
		}

		friend std::ostream& operator<<(std::ostream& out, const HDR_rgb& rgb) {
			return out << "[ r=" << rgb.r() << ", g=" << rgb.g() << ", b=" << rgb.b() << " ]";
		}
	private:
		storage_type data_;
	};

	const HDR_rgb
		BLACK(0.0, 0.0, 0.0),
		WHITE(1.0, 1.0, 1.0),
		RED  (1.0, 0.0, 0.0),
		GREEN(0.0, 1.0, 0.0),
		BLUE (0.0, 0.0, 1.0);

}