
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


	m_Scene->DrawPath1();

	CDebugGui::Begin();
	CRenderer::Begin();

	m_Scene->Draw();

	//light.Enable = false;



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
