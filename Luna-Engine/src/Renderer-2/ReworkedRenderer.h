#pragma once
namespace Luna
{
	class ReworkedRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		static void Render();
	};
}

