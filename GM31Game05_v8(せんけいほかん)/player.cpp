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
	{
		//shader_pop = CRenderer::GetShader<CPop>();
		//shader_lit = CRenderer::GetShader<CLit>();
		//shader_fog = CRenderer::GetShader<CFog>();
		shader_shadowM = CRenderer::GetShader<CShadowM>();
	}

	m_AnimationModel->Load("asset\\model\\Reaction.fbx");
	m_AnimationModel->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");
	m_AnimationModel->LoadAnimation("asset\\model\\Reaction.fbx", "React");
	m_AnimationModel->LoadAnimation("asset\\model\\Akai_Idle.fbx", "Idle");
	m_AnimationModel->LoadAnimation("asset\\model\\Sitting.fbx", "Sit");
	m_AnimationModel->LoadAnimation("asset\\model\\Cat.fbx", "Cat");


	m_Position = D3DXVECTOR3(0.0f, 2.0f,-3.0f);
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
	m_AnimationModel->Update("Sit", "Run", m_BlendRate, m_Frame/2);
	m_Frame++;

	//m_Rotation.y += 0.01f;
	D3DXVECTOR3 forward = GetForward();//前方向ベクトル
	
	if (CInput::GetKeyPress('D'))	//VK_SHIFTでSHIFTに対応　VK_SHIFTの定義に移動で大体キーコードわかる
	{
		m_Rotation.y += 0.05f;
		//m_BlendRate += 0.05f;
		//m_Position.x -= 0.1f;
	
	}
	else if (CInput::GetKeyPress('A'))
	{
		m_Rotation.y -= 0.05f;
		//m_BlendRate += 0.05f;
		//m_Position.x += 0.1f;
	}
	else if (CInput::GetKeyPress('W'))
	{
		m_Position -= forward * 0.15f;
		m_BlendRate += 0.05f;
		//m_Position.z -= 0.1f;
	}
	else if (CInput::GetKeyPress('S'))
	{
		m_Position += forward * 0.15f;
		m_BlendRate += 0.05f;
		//m_Position.z += 0.1f;
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

	//メッシュフィールド高さ取得
	CMeshField* meshField = CManager::GetScene()->GetGameObject<CMeshField>(1);

	m_Position.y = meshField->GetHeight(m_Position);


}

void CPlayer::Draw()
{
	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	//D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	//world = scale * rot * trans;
	world = rot * scale * trans ;
	{
		shader_shadowM->SetWorldMatrix(&world);	//shadowMapping
	}

	CRenderer::SetShader(shader_shadowM);

	//シャドウバッファテクスチャをセット
	ID3D11ShaderResourceView* shadowDepthTexture = CRenderer::GetShadowDepthTexture();//-追加
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);//-追加

	//m_Model->Draw();
	m_AnimationModel->Draw();
}

void CPlayer::DrawPath1()
{
	//SetLight
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//-----------ライトをカメラとみなした行列を作成
	D3DXMatrixLookAtLH(&light.ViewMatrix, &D3DXVECTOR3(-10.0f, 10.0f, -10.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//-----------ライト用のプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 30.0f);

	shader_shadowM->SetLight(light);
	CRenderer::BeginDepth();

	shader_shadowM->SetViewMatrix(&light.ViewMatrix);
	shader_shadowM->SetProjectionMatrix(&light.ProjectionMatrix);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	//D3DXMatrixRotationQuaternion(&rot, &m_Quaternion);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	//world = scale * rot * trans;
	world = rot * scale * trans;
	{
		shader_shadowM->SetWorldMatrix(&world);	//shadowMapping
	}

	CRenderer::SetShader(shader_shadowM);

	//シャドウバッファテクスチャをセット
	ID3D11ShaderResourceView* shadowDepthTexture = CRenderer::GetShadowDepthTexture();//-追加
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);//-追加

	m_AnimationModel->Draw();
}
