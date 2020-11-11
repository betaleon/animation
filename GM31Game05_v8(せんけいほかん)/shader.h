#pragma once
#include "main.h"
#include "renderer.h"

class CShader
{
	friend class CRenderer;
protected:
	ID3D11VertexShader*     m_VertexShader = NULL;
	ID3D11PixelShader*      m_PixelShader = NULL;
	ID3D11InputLayout*      m_VertexLayout = NULL;
	ID3D11Buffer*			m_WorldBuffer = NULL;
	ID3D11Buffer*			m_ViewBuffer = NULL;
	ID3D11Buffer*			m_ProjectionBuffer = NULL;
	ID3D11Buffer*			m_MaterialBuffer = NULL;
	ID3D11Buffer*			m_LightBuffer = NULL;

public:
	virtual void Init() {}
	virtual void UnInit() {}
	
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
	};

	virtual void SetWorldMatrix(D3DXMATRIX * WorldMatrix) { 
		D3DXMATRIX world;
		D3DXMatrixTranspose(&world, WorldMatrix);
		CRenderer::GetDeviceContext()->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0); 
	};

	virtual void SetViewMatrix(D3DXMATRIX * ViewMatrix) {
		D3DXMATRIX view;
		D3DXMatrixTranspose(&view, ViewMatrix);
		CRenderer::GetDeviceContext()->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
	};

	virtual void SetProjectionMatrix(D3DXMATRIX * ProjectionMatrix) {
		D3DXMATRIX projection;
		D3DXMatrixTranspose(&projection, ProjectionMatrix);
		CRenderer::GetDeviceContext()->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
	};

	virtual void SetMaterial(MATERIAL Material) {
		CRenderer::GetDeviceContext()->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
	};

	virtual void SetLight(LIGHT Light) {
		CRenderer::GetDeviceContext()->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
	};
};