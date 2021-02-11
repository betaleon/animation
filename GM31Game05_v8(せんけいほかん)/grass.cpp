#include "main.h"
#include "renderer.h"
#include "grass.h"
#include "meshField.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"

#define SIZE 3.0f
#define GRASS_MOUNT 1100

void CGrass::Init()
{
	shader_instancing = CRenderer::GetShader<CInstancing>();
	//shader_lit = (CLit*)CRenderer::GetShader();
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-SIZE, SIZE, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//1�ȏ�ɂ���ƃ��[�v����

	vertex[1].Position = D3DXVECTOR3(SIZE, SIZE, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-SIZE, -SIZE, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SIZE, -SIZE, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				//�d�v
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;	//�d�v

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/grass.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);
	{
		CMeshField* meshField = CManager::GetScene()->GetGameObject<CMeshField>(1);

		D3DXVECTOR3* pos = new D3DXVECTOR3[GRASS_MOUNT];

		int i = 0;
		for (int z = sqrt(GRASS_MOUNT) / 2 ; z > -sqrt(GRASS_MOUNT)/2; z--)
		{
			for (int x = sqrt(GRASS_MOUNT) / 2; x > -sqrt(GRASS_MOUNT) / 2; x--)
			{
				pos[i] = D3DXVECTOR3(2.0f * x , 0,  z);
				pos[i].y = meshField->GetHeight(pos[i]);	//+ SIZE / 2.0f;
				i++;
			}
		}

		//�X�g���N�`���o�b�t�@����
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;				//�d�v
		bd.ByteWidth = sizeof(D3DXVECTOR3) * GRASS_MOUNT;
		bd.StructureByteStride = sizeof(D3DXVECTOR3);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pos;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_PositionBuffer);

		delete[] pos;

		//�V�F�[�_�[���\�[�X�r���[����
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = GRASS_MOUNT;

		CRenderer::GetDevice()->CreateShaderResourceView(m_PositionBuffer, &srvd, &m_PositionSRV);

	}
	
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

}

void CGrass::Uninit()
{

	m_VertexBuffer->Release();
	m_Texture->Release();

	m_PositionBuffer->Release();

}

void CGrass::Update()
{
	//CMeshField* meshField = CManager::GetScene()->GetGameObject<CMeshField>(1);


}

void CGrass::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;

	
   
	//CCamera* camera = CManager::GetScene()->GetGameObject<CCamera>(0);
	////�ȉ��r���{�[�h�ݒ�
	//D3DXMATRIX view = camera->GetViewMatrix();
	//D3DXMATRIX invView;
	//D3DXMatrixInverse(&invView, NULL, &view);//�t�s��
	//invView._41 = 0.0f;
	//invView._42 = 0.0f;
	//invView._43 = 0.0f;

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	//world = scale * invView * trans;
	world = scale * rot * trans;
	shader_instancing->SetWorldMatrix(&world);


	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	shader_instancing->SetMaterial(material);

	//�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�X�g���N�`���[�h�o�b�t�@�ݒ�
	CRenderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_PositionSRV);

	//�v���~�e�B�u�g�|���W�^
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//SetShader
	CRenderer::SetShader(shader_instancing);

	//�|���S���`��
	//CRenderer::GetDeviceContext()->Draw(4, 0);
	//CRenderer::SetDepthEnable(false);
	CRenderer::GetDeviceContext()->DrawInstanced(4, GRASS_MOUNT, 0, 0);
	//CRenderer::SetDepthEnable(true);
}