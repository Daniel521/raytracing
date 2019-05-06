#pragma once
#include <fstream>
#include <vector>
#include "Triangle_Object.h"
#include "HDR_RGB.h"
#include "OBJ_Loader.h"

namespace RT {

	class Mesh {
	public:
		using storage_type = std::vector<Triangle_Object*>;
		using iterator = storage_type::iterator;
		using const_iterator = storage_type::const_iterator;
	public:
		Mesh() = delete;
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(std::string filename, const HDR_rgb& color = HDR_rgb(), double shininess = 0.1) {
			objl::Loader loader;
			bool loadout = loader.LoadFile(filename);
			assert(loadout);
			for (size_t i = 0; i < loader.LoadedMeshes.size(); ++i) {
				objl::Mesh mesh = loader.LoadedMeshes[i];
				for (size_t j = 0; j < mesh.Indices.size(); j+=3) {
					triangles_.push_back(new Triangle_Object(
						Point({ mesh.Vertices[mesh.Indices[j]].Position.X,   mesh.Vertices[mesh.Indices[j]].Position.Y,   mesh.Vertices[mesh.Indices[j]].Position.Z }),
						Point({ mesh.Vertices[mesh.Indices[j+1]].Position.X, mesh.Vertices[mesh.Indices[j+1]].Position.Y, mesh.Vertices[mesh.Indices[j+1]].Position.Z }),
						Point({ mesh.Vertices[mesh.Indices[j+2]].Position.X, mesh.Vertices[mesh.Indices[j+2]].Position.Y, mesh.Vertices[mesh.Indices[j+2]].Position.Z }), 
						color, shininess));
				}
			}
		}
		~Mesh() {
			for (size_t i = 0; i < triangles_.size(); ++i) {
				delete triangles_[i];
			}
		}


		iterator begin() { return triangles_.begin(); }
		iterator end() { return triangles_.end(); }
		const_iterator begin() const { return triangles_.begin(); }
		const_iterator end() const { return triangles_.end(); }
		size_t size() const { return triangles_.size(); }
		bool is_empty() const { return triangles_.empty(); }
		const Triangle_Object& operator[](size_t i) const { assert(i < triangles_.size()); return *triangles_[i]; }
		Triangle_Object& operator[](size_t i) { assert(i < triangles_.size()); return *triangles_[i]; }


	private:
		storage_type triangles_;
	};

}