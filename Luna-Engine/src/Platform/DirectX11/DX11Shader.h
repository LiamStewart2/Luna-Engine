#pragma once

#include "../../Renderer-2/IShader.h"

struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace Luna
{
	class DX11Shader : public IShader
	{
	public:
		DX11Shader(const std::string& filepath);
		virtual ~DX11Shader();

		void CompileShader(const std::string& filepath);

		void Bind() override;
		void Unbind() override;
	private:
		void Release();

		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;

		ID3D11InputLayout* m_InputLayout = nullptr;
	};
}