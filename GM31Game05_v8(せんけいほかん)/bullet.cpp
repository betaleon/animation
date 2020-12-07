#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "bullet.h"
#include "input.h"
#include "scene.h"

CModel* CBullet::m_Model;

void CBullet::Load()
{
	m_Model = new CModel();
	m_Model->Load("asset\\model\\cube.obj");
}

void CBullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}


void CBullet::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);

	shader_lit = CRenderer::GetShader<CLit>();
}

void CBullet::Uninit()
{

}

void CBullet::Update()
{

	m_Position.z += 0.1f;

	if (m_Position.z > 10.0f)
	{
		SetDestroy();
		return;
	}

	CScene* scene = CManager::GetScene();
	std::vector<CEnemy*> enemyList = scene->GetGameObjects<CEnemy>(1);

	for (CEnemy* enemy : enemyList)
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();

		D3DXVECTOR3 direction = m_Position - enemyPosition;
		float length = D3DXVec3Length(&direction);

		if (length < 1.5f)
		{
			enemy->SetDestroy();
			SetDestroy();
			return;
		}
	}

}

void CBullet::Draw()
{
	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	shader_lit->SetWorldMatrix(&world);

	CRenderer::SetShader(shader_lit);

	m_Model->Draw();
}