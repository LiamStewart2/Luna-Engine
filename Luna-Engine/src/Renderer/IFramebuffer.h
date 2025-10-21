#pragma once

#include <memory>
#include <vector>

namespace Luna
{
	enum FramebufferTextureFormat
	{
		None = 0,
		RGBA8,
		RGBA16F,
	};
	enum DepthTextureFormat
	{
		DEPTH24STENCIL8,
		DEPTH32F,
	};

	struct FramebufferSpecification
	{
		unsigned int m_Width = 0;
		unsigned int m_Height = 0;
		std::vector<FramebufferTextureFormat> m_ColorAttachments;
		DepthTextureFormat m_DepthAttachment;
		bool m_SwapChainTarget = false;
	};

	class IFramebuffer
	{
	public:
		virtual ~IFramebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(const unsigned int& width, const unsigned int& height) = 0;

		virtual void Clear(const float color[4]) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void* GetColorAttachment(int index = 0) = 0;
		virtual void* GetDepthAttachment() = 0;

		static std::shared_ptr<IFramebuffer> Create(const FramebufferSpecification& spec);
	};
}