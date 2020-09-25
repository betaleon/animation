#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
//#include "model.h"
#include "animation_model.h"
#include "player.h"

void CPlayer ::Init()
{
	//m_Model = new CModel();
	//m_Model->Load("asset\\model\\horse_v3.obj");
	m_AnimationModel = new CAnimationModel();
	//m_AnimationModel->Load("asset\\model\\Akai_Idle.fbx");
	//m_AnimationModel->LoadAnimation("asset\\model\\Akai_Idle.fbx","Idle");

	m_AnimationModel->Load("asset\\model\\Reaction.fbx");
	m_AnimationModel->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");
	m_AnimationModel->LoadAnimation("asset\\model\\Reaction.fbx", "React");
	m_AnimationModel->LoadAnimation("asset\\model\\Akai_Idle.fbx", "Idle");
	m_AnimationModel->LoadAnimation("asset\\model\\Sitting.fbx", "Sit");
	m_AnimationModel->LoadAnimation("asset\\model\\Cat.fbx", "Cat");


	m_Position = D3DXVECTOR3(0.0f, 2.0f, -3.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	//D3DXQuaternionIdentity(&m_Quaternion);

}

void CPlayer::Uninit()
{

	//m_Model->Unload();
	//delete m_Model;
	m_AnimationModel->Unload();
	delete m_AnimationModel;

}

void CPlayer::Update()
{

	//m_AnimationModel->Update("Idle", "Run", m_BlendRate, m_Frame/2);
	m_AnimationModel->Update("Run", "Sit", m_BlendRate, m_Frame/2);
	m_Frame++;

	//m_Rotation.y += 0.01f;
	
	
	if (CInput::GetKeyPress('A'))	//VK_SHIFTでSHIFTに対応　VK_SHIFTの定義に移動で大体キーコードわかる
	{
		m_BlendRate += 0.05f;
		m_Position.x -= 0.1f;
	
	}
	else if (CInput::GetKeyPress('D'))
	{
		m_BlendRate += 0.05f;
		m_Position.x += 0.1f;
	}
	else if (CInput::GetKeyPress('S'))
	{
		m_BlendRate += 0.05f;
		m_Position.z -= 0.1f;
	}
	else if (CInput::GetKeyPress('W'))
	{
		m_BlendRate += 0.05f;
		m_Position.z += 0.1f;
	}
	else
	{
		m_BlendRate -= 0.03f;
		//m_AnimationModel->Update("Idle", m_Frame);
	}

	if (CInput::GetKeyTrigger(VK_SPACE))
	{
		CScene* scene = CManager::GetScene();
		scene->AddGameObject<CBullet>(1)->SetPosition(m_Position);
	}

	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;
	if (m_BlendRate < 0.0f)
		m_BlendRate = 0.0f;

	m_Frame++;
}

void CPlayer::Draw()
{
	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	//D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	CRenderer::SetWorldMatrix(&world);

	//m_Model->Draw();
	m_AnimationModel->Draw();
}