#pragma once
#include "main.h"
#include "renderer.h"

class CShader
{
protected:
	ID3D11VertexShader*     m_VertexShader = NULL;
	ID3D11PixelShader*      m_PixelShader = NULL;
	ID3D11InputLayout*      m_VertexLayout = NULL;
	ID3D11Buffer*			m_WorldBuffer = NULL;
	ID3D11Buffer*			m_ViewBuffer = NULL;
	ID3D11Buffer*			m_ProjectionBuffer = NULL;
	ID3D11Buffer*			m_MaterialBuffer = NULL;
	ID3D11Buffer*			m_LightBuffer = NULL;

	friend class CRenderer;
public:
	ID3D11GeometryShader*	m_GeometryShader = NULL;
	virtual void Init() {}
	virtual void UnInit() {
		if (m_WorldBuffer) m_WorldBuffer->Release();
		if (m_ViewBuffer) m_ViewBuffer->Release();
		if (m_ProjectionBuffer) m_ProjectionBuffer->Release();
		if (m_LightBuffer) m_LightBuffer->Release();
		if (m_MaterialBuffer) m_MaterialBuffer->Release();

		if (m_VertexLayout) m_VertexLayout->Release();
		if (m_VertexShader) m_VertexShader->Release();
		if (m_GeometryShader) m_GeometryShader->Release();
		if (m_PixelShader) m_PixelShader->Release();
	}

	virtual void UpdateConstantBuffers(){}
	
	virtual void SetWorldViewProjection2D() {
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranspose(&world, &world);

		CRenderer::GetDeviceContext()->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

		D3DXMATRIX view;
		D3DXMatrixIdentity(&view);
		D3DXMatrixTranspose(&view, &view);
		CRenderer::GetDeviceContext()->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

		D3DXMATRIX projection;
		D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
		D3DXMatrixTranspose(&projection, &projection);
		CRenderer::GetDeviceContext()->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
	}

	virtual void SetWorldMatrix(D3DXMATRIX * WorldMatrix) {}

	virtual void SetViewMatrix(D3DXMATRIX * ViewMatrix) {}

	virtual void SetProjectionMatrix(D3DXMATRIX * ProjectionMatrix) {}

	virtual void SetMaterial(MATERIAL Material) {}

	virtual void SetLight(LIGHT Light) {}
	
	virtual void PS_SetTexture(ID3D11ShaderResourceView* texture){}
	virtual void PS_SetSamplerState(ID3D11SamplerState* sampler){}

};