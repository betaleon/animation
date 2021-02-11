#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "animation_model.h"
#include "player.h"
#include "meshField.h"
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

	//third parson view �����_���v���C���[�̑O���ɂ��āA�|�W�V�������v���C���[���班�����ɂ��炷�Ƃ��Q�[���������シ��
	D3DXVECTOR3 forward = player->GetForward();
	m_Position = m_Target - -forward * 10.0f + D3DXVECTOR3(0.0f, 5.0f, 0.0f);

	//���b�V���t�B�[���h�����擾
	CMeshField* meshField = CManager::GetScene()->GetGameObject<CMeshField>(1);
	if (m_Position.y <= meshField->GetHeight(m_Position) + 2.0f)
	{
		m_Position.y = meshField->GetHeight(m_Position) + 2.0f;
	}
}

void CCamera::Draw()
{
	//�r���[�}�g���N�X�ݒ�
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	//shader_lit->SetViewMatrix(&m_viewMatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.5f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	
	//shader_lit->SetProjectionMatrix(&m_ProjectionMatrix);
	auto shaders = CRenderer::GetShaders();

	for (auto shader : shaders)
	{
		shader->SetViewMatrix(&m_viewMatrix);
		shader->SetProjectionMatrix(&m_ProjectionMatrix);
	}
}

