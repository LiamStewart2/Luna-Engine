#pragma once

#include "../../Renderer-2/Framebuffer.h"

namespace Luna
{
	class DX11Framebuffer : public IFramebuffer
	{
	public:
		void Bind() override;
		void Unbind() override;
		void Resize(const unsigned int& width, const unsigned int& height) override;

		void Clear(const float color[4]) override;

		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		void* GetColorAttachment(int index = 0) override;
		void* GetDepthAttachment() override;
	};
}