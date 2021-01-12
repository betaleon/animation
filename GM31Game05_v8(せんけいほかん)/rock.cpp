#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "rock.h"
#include "input.h"
#include "scene.h"

void CRock::Init()
{
	m_Model = new CModel();
	m_Model->Load("asset\\model\\Rock1.obj");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);

	shader_fog = CRenderer::GetShader<CFog>();
}

void CRock::Uninit()
{

	m_Model->Unload();
	delete m_Model;

}

void CRock::Update()
{
	//メッシュフィールド高さ取得
	CMeshField* meshField = CManager::GetScene()->GetGameObject<CMeshField>(1);

	m_Position.y = meshField->GetHeight(m_Position);
}

void CRock::Draw()
{
	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	shader_fog->SetWorldMatrix(&world);

	CRenderer::SetShader(shader_fog);

	m_Model->Draw();
}