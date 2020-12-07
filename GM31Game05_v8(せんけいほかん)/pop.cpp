#include "main.h"
#include "renderer.h"
#include "pop.h"
#include <io.h>


void CPop::Init()
{
	auto device = CRenderer::GetDevice();
	auto deviceContext = CRenderer::GetDeviceContext();

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	device->CreateSamplerState(&samplerDesc, &samplerState);
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	//頂点シェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen("pop_VS.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		device->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);


		// 入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);
		
		device->CreateInputLayout(layout,
			numElements,
			buffer,
			fsize,
			&m_VertexLayout);

		delete[] buffer;
	}
	//ジオメトリシェーダ生成
	{
		FILE* file;
		long int fsize;
		
		file = fopen("pop_GS.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);
		
		device->CreateGeometryShader(buffer, fsize, NULL, &m_GeometryShader);
		
		delete[] buffer;
	}

	// ピクセルシェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen("pop_PS.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		device->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

		delete[] buffer;
	}

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	device->CreateBuffer(&hBufferDesc, NULL, &m_WorldBuffer);
	device->CreateBuffer(&hBufferDesc, NULL, &m_ViewBuffer);
	device->CreateBuffer(&hBufferDesc, NULL, &m_ProjectionBuffer);

	device->CreateBuffer(&hBufferDesc, NULL, &m_GS_WorldBuffer);
	device->CreateBuffer(&hBufferDesc, NULL, &m_GS_ViewBuffer);
	device->CreateBuffer(&hBufferDesc, NULL, &m_GS_ProjectionBuffer);

	hBufferDesc.ByteWidth = sizeof(MATERIAL);
	device->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);

	hBufferDesc.ByteWidth = sizeof(LIGHT);
	device->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);



	//UpdateConstantBuffers();

}

void CPop::UnInit()
{
	CShader::UnInit();
}