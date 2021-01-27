
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "debugimgui.h"
#include "imgui.h"
#include "animation_model.h"
#include "player.h"
#include "shadowMapping.h"

CScene* CManager::m_Scene = nullptr;

//CShadowM* g_shadowM;

void CManager::Init()
{

	CRenderer::Init();
	CDebugGui::Init();
	CInput::Init();

	m_Scene = new CScene();
	m_Scene->Init();

}

void CManager::Uninit()
{

	m_Scene->Uninit();
	delete m_Scene;

	CInput::Uninit();
	CDebugGui::Finalize();
	CRenderer::Uninit();
}

void CManager::Update()
{
	CInput::Update();
	CDebugGui::Update();

	m_Scene->Update();
}

void CManager::Draw()
{
//LIGHT light;
//light.Enable = true;
//light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
//D3DXVec4Normalize(&light.Direction, &light.Direction);
//light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
//light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
////-----------ライトをカメラとみなした行列を作成
//D3DXMatrixLookAtLH(&light.ViewMatrix, &D3DXVECTOR3(-10.0f, 10.0f, -10.0f),
//	&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
////-----------ライト用のプロジェクション行列を作成
//D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
//	(float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 30.0f);

	//g_shadowM->SetLight(light);
	//g_shadowM->BeginDepth();

	//g_shadowM->SetViewMatrix(&light.ViewMatrix);
	//g_shadowM->SetProjectionMatrix(&light.ProjectionMatrix);

	//m_Scene->DrawPath1();

	CDebugGui::Begin();
	CRenderer::Begin();

	m_Scene->Draw();

	//light.Enable = false;
	//g_shadowM->SetLight(light);



	//ImGui
	ImGui::SetNextWindowPos(ImVec2(20, 20));

	ImGui::Begin("Debug");
	
	CPlayer* pPlayer = m_Scene->GetGameObject<CPlayer>(1);
	if (pPlayer != NULL)
	{
		ImGui::Text("PlayerSize");
		static float a = 0.01f;
		pPlayer->SetScale(D3DXVECTOR3(a, a, a));
		ImGui::SliderFloat("a", &a, 0.00f, 0.02f);
	}
	
	ImGui::End();

	CDebugGui::End();
	CRenderer::End();
}
