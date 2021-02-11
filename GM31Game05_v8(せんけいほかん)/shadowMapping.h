#pragma once
#pragma once
#include "shader.h"

class CShadowM : public CShader
{
public:
	void Init() override;
	void UnInit() override;
	void UpdateConstantBuffers() override
	{
		auto deviceContext = CRenderer::GetDeviceContext();

		// set input layout
		deviceContext->IASetInputLayout(m_VertexLayout);

		// set constant buffers
		deviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);
		deviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);
		deviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);
		deviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
		deviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	};

	//void BeginDepth()//�V�K�֐��ǉ�
	//{
	//	auto deviceContext = CRenderer::GetDeviceContext();
	//
	//	//�V���h�E�o�b�t�@��[�x�o�b�t�@�ɐݒ肵�A���e��1�œh��Ԃ�
	//	deviceContext->OMSetRenderTargets(0, NULL, m_ShadowDepthStencilView);
	//	deviceContext->ClearDepthStencilView(m_ShadowDepthStencilView,
	//		D3D11_CLEAR_DEPTH, 1.0f, 0);
	//}

	//static ID3D11ShaderResourceView* GetShadowDepthTexture()
	//{
	//	return m_ShadowDepthShaderResourceView;
	//}

	void SetWorldMatrix(D3DXMATRIX *WorldMatrix) override
	{
		D3DXMATRIX world;
		D3DXMatrixTranspose(&world, WorldMatrix);
		CRenderer::GetDeviceContext()->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
	}

	void SetViewMatrix(D3DXMATRIX *ViewMatrix) override
	{
		D3DXMATRIX view = *ViewMatrix;
		D3DXMatrixTranspose(&view, ViewMatrix);
		CRenderer::GetDeviceContext()->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
	}

	void SetProjectionMatrix(D3DXMATRIX *ProjectionMatrix) override
	{
		D3DXMATRIX projection = *ProjectionMatrix;
		D3DXMatrixTranspose(&projection, ProjectionMatrix);
		CRenderer::GetDeviceContext()->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
	}

	void SetMaterial(MATERIAL Material) override
	{
		CRenderer::GetDeviceContext()->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
	}

	void SetLight(LIGHT Light) override
	{
		D3DXMatrixTranspose(&Light.ViewMatrix, &Light.ViewMatrix);//�ǉ�
		D3DXMatrixTranspose(&Light.ProjectionMatrix, &Light.ProjectionMatrix);//�ǉ�

		CRenderer::GetDeviceContext()->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
	}

	void PS_SetTexture(ID3D11ShaderResourceView* texture) override
	{
		CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	}

	void PS_SetSamplerState(ID3D11SamplerState* sampler) override
	{
		CRenderer::GetDeviceContext()->PSSetSamplers(0, 1, &sampler);
	}

	void PS_SetShadowSamplerState(ID3D11SamplerState* sampler) 
	{
		CRenderer::GetDeviceContext()->PSSetSamplers(1, 1, &sampler);
	}
};