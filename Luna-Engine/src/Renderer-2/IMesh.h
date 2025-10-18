#pragma once

#include <string>
#include <vector>
#include <memory>
#include "GLM/glm.hpp"

namespace Luna
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TextureCoordinate;
		glm::vec3 Normal;
	};

	class IMesh
	{
	public:
		virtual ~IMesh() = default;
		std::string m_Path;

		virtual void BuildMesh() = 0;
		virtual void BindMesh() = 0;

		// BuildMesh must be used after this to update the graphics API buffers
		void SetVertexIndexData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) 
		{ 
			this->m_VertexData = vertices;
			this->m_IndexData = indices;
		}

		static std::shared_ptr<IMesh> Create(const std::vector<Vertex>& vertices = {}, const std::vector<unsigned int>& indices = {});
	protected:
		std::vector<Vertex> m_VertexData;
		std::vector<unsigned int> m_IndexData;
	};
}