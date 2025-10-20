#include "../../Renderer-2/ITexture.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace Luna
{
	class DX11Texture : public ITexture
	{
	public:
		DX11Texture(const TexturePacket& textureData);
		virtual ~DX11Texture();

		virtual void BuildTexture() override;
		virtual void BindTexture(unsigned int slot = 0) const override;
		virtual void* GetTextureReference() override;
	private:
		virtual void Release();
		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11ShaderResourceView* m_TextureView = nullptr;
		ID3D11SamplerState* m_SamplerState = nullptr;
	};
}