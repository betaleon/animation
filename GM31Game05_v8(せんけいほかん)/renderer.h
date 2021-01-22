#pragma once
#include <vector>
#include <typeinfo>

class CShader;

// 頂点構造体
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};



// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//16byte境界用
};




// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture*	Texture;
};


// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte境界用
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;

	D3DXMATRIX ViewMatrix;//追加
	D3DXMATRIX ProjectionMatrix;//追加
};


class CVertexBuffer;
class CIndexBuffer;
class CTexture;

class CRenderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_D3DDevice;
	static ID3D11DeviceContext*    m_ImmediateContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11DepthStencilView* m_ShadowDepthStencilView;//追加
	static ID3D11ShaderResourceView*m_ShadowDepthShaderResourceView;//追加

	static std::vector<CShader*> m_shaders;


public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetShader(CShader* shader);

	static ID3D11Device* GetDevice( void ){ return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }

	//ShadowMapping用
	static void BeginDepth();//追加
	static ID3D11ShaderResourceView* GetShadowDepthTexture() //追加
	{
		return m_ShadowDepthShaderResourceView;
	}

	template <typename T>
	static T* GetShader()
	{
		for (auto shader : m_shaders)
		{
			if (typeid(*shader) == typeid(T))
			{
				return static_cast<T*> (shader);
			}
		}

		return nullptr;
	}

	static std::vector<CShader*> GetShaders() { return m_shaders; }
};
