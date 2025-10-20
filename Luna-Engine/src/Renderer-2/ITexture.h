#pragma once

#include <string>
#include <memory>

namespace Luna
{
	struct TexturePacket
	{
		void* buffer = nullptr;
		std::string path;
		int width = 0; int height = 0;
		int channels = 0;
		int bytesPerPixel = 0;
	};

	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void BuildTexture() = 0;
		virtual void BindTexture(unsigned int slot = 0) const = 0;
		virtual void* GetTextureReference() = 0;

		void SetData(const TexturePacket& texturePacket)
		{
			m_TextureData = texturePacket;
		}
		TexturePacket* GetTexturePacket() 
		{
			return &m_TextureData;
		}

		static std::shared_ptr<ITexture> Create(const TexturePacket& textureData);
	protected:
		TexturePacket m_TextureData;
	};
}