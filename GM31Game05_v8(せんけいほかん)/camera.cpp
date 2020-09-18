#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

void CCamera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 4.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f,1.0f, 3.0f);

}

void CCamera::Uninit()
{

}

void CCamera::Update()
{

}

void CCamera::Draw()
{
	//ビューマトリクス設定
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	CRenderer::SetViewMatrix(&viewMatrix);

	//プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.5f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	CRenderer::SetProjectionMatrix(&projectionMatrix);
}