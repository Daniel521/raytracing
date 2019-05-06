#pragma once
#include "Image.h"
#include <fstream>

namespace RT {

	void ppm_writer(const RT::Image& image, std::string title) {
		std::ofstream outfile;
		outfile.open(title);
		outfile << "P3" << std::endl;
		outfile << image.x_resolution() << ' ' << image.y_resolution() << ' ' << "255" << std::endl;
		outfile << image;
		outfile.close();
	}

}