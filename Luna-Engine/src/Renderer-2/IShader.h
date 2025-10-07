#pragma once

#include <memory>
#include <string>

namespace Luna
{
	class IShader
	{
	public:
		virtual ~IShader() = default;
		std::string path;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static std::shared_ptr<IShader> Create(const std::string& filepath);
	};
}