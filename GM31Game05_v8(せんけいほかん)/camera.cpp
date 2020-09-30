#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "animation_model.h"
#include "player.h"
#include "scene.h"

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
	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(1);
	m_Target = player->GetPosition();

	//top view
	//m_Position = m_Target + D3DXVECTOR3(0.0f, 20.0f, -10.0f);

	//third parson view 注視点をプレイヤーの前方にして、ポジションをプレイヤーから少し横にずらすとよりゲーム性が向上する
	D3DXVECTOR3 forward = player->GetForward();
	m_Position = m_Target - -forward * 10.0f + D3DXVECTOR3(0.0f, 5.0f, 0.0f);
}

void CCamera::Draw()
{
	//ビューマトリクス設定
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	CRenderer::SetViewMatrix(&m_viewMatrix);

	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.5f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
}