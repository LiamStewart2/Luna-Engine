#pragma once

namespace Luna
{ 
	enum RendererAPIType
	{
		None,
		DirectX11,
		OpenGL
	};

	class IRendererAPI
	{
	public:
		virtual ~IRendererAPI() = default;

		virtual void Init() = 0;

		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void RenderIndexed(unsigned int count) = 0;

		static RendererAPIType GetAPI() { return s_API; }

	private:
		static RendererAPIType s_API;
	};
}