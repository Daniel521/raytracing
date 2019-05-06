#pragma once
#include "Abstract_Shader.h"
#include "Intersection.h"
#include "Scene.h"
#include "Camera.h"

namespace RT {

	class Flat_Shader : public Abstract_Shader {
	public:
		HDR_rgb shade(const Scene& scene, const Camera& camera, const Intersection& intersection) const {
			return intersection.object().color();
		}
	};

}