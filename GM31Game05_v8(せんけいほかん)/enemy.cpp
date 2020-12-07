#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "enemy.h"
#include "input.h"

void CEnemy::Init()
{
	m_Model = new CModel();
	m_Model->Load("asset\\model\\horse_v3.obj");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	shader_lit = CRenderer::GetShader<CLit>();
}

void CEnemy::Uninit()
{

	m_Model->Unload();
	delete m_Model;

}

void CEnemy::Update()
{
	

}

void CEnemy::Draw()
{
	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	shader_lit->SetWorldMatrix(&world);

	m_Model->Draw();
}