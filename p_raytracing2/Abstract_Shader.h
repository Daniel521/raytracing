#pragma once

namespace RT {

	class Camera;
	class Scene;
	class HDR_rgb;
	class Intersection;

	class Abstract_Shader {
	public:
		virtual HDR_rgb shade(const Scene& scene, const Camera& camera, const Intersection& intersection) const = 0;
		virtual ~Abstract_Shader() = default;
	};

}