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
		glm::vec3 Tangent;
		glm::vec3 Bitangent;

		std::size_t operator==(const Vertex& other) const
		{
			return Position == other.Position &&
				TextureCoordinate == other.TextureCoordinate &&
				Normal == other.Normal;
		}

		struct Hash
		{
			std::size_t operator()(const Vertex& v) const {
				std::size_t h1 = std::hash<float>{}(v.Position.x);
				std::size_t h2 = std::hash<float>{}(v.Position.y);
				std::size_t h3 = std::hash<float>{}(v.Position.z);
				std::size_t h4 = std::hash<float>{}(v.TextureCoordinate.x);
				std::size_t h5 = std::hash<float>{}(v.TextureCoordinate.y);
				std::size_t h6 = std::hash<float>{}(v.Normal.x);
				std::size_t h7 = std::hash<float>{}(v.Normal.y);
				std::size_t h8 = std::hash<float>{}(v.Normal.z);
				return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^
					(h5 << 4) ^ (h6 << 5) ^ (h7 << 6) ^ (h8 << 7);
			}
		};
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
		int GetIndexCount() {return m_IndexData.size(); }

		static std::shared_ptr<IMesh> Create(const std::vector<Vertex>& vertices = {}, const std::vector<unsigned int>& indices = {});

	protected:
		std::vector<Vertex> m_VertexData;
		std::vector<unsigned int> m_IndexData;
	};
}