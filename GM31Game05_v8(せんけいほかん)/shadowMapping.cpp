#include "main.h"
#include "renderer.h"
#include "shadowMapping.h"
#include <io.h>

void CShadowM::Init()
{
	auto device = CRenderer::GetDevice();
	auto deviceContext = CRenderer::GetDeviceContext();

	// 頂点シェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen("shadowMappingVS.cso", "rb");
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



	// ピクセルシェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen("shadowMappingPS.cso", "rb");
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
	deviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	device->CreateBuffer(&hBufferDesc, NULL, &m_ViewBuffer);
	deviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	device->CreateBuffer(&hBufferDesc, NULL, &m_ProjectionBuffer);
	deviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	device->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);
	deviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	hBufferDesc.ByteWidth = sizeof(LIGHT);

	device->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);
	deviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);

	// 入力レイアウト設定
	deviceContext->IASetInputLayout(m_VertexLayout);
}

void CShadowM::UnInit()
{
	CShader::UnInit();
}
