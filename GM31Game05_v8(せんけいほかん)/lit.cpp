#include "main.h"
#include "renderer.h"
#include "lit.h"
#include <io.h>

void CLit::Init()
{
	// 頂点シェーダ生成
	{
		FILE* file;
		long int fsize;

		file = fopen("vertexShader.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);
		CRenderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_VertexShader);


		// 入力レイアウト生成
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(layout);

		CRenderer::GetDevice()->CreateInputLayout(layout,
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

		file = fopen("pixelShader.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		CRenderer::GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_PixelShader);

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

	CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_WorldBuffer);
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ViewBuffer);
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_ProjectionBuffer);
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	hBufferDesc.ByteWidth = sizeof(LIGHT);

	CRenderer::GetDevice()->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);
	CRenderer::GetDeviceContext()->VSSetConstantBuffers(4, 1, &m_LightBuffer);





	// 入力レイアウト設定
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
}

void CLit::UnInit()
{
	// オブジェクト解放
	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}
