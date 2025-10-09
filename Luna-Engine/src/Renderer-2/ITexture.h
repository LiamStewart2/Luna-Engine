#pragma once

#include <string>
#include <memory>

namespace Luna
{
	class ITexture
	{
	public:
		virtual ~ITexture() = default;
		std::string path;

		virtual void BuildTexture() = 0;
		virtual void BindTexture(unsigned int slot = 0) const = 0;

		virtual void SetData(void* data, unsigned int size) = 0;

		static std::shared_ptr<ITexture> Create(const std::string& path);
	};
}