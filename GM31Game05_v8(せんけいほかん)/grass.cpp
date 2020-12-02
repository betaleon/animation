#include "main.h"
#include "renderer.h"
#include "grass.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"

#define SIZE 3.0f
void CGrass::Init()
{
	//shader_instancing = (CInstancing*)CRenderer::GetInstancingShader();
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
		D3DXVECTOR3* pos = new D3DXVECTOR3[1000000];

		int i = 0;
		for (int x = 0; x < 1000; x++)
		{
			for (int z = 0; z < 1000; z++)
			{
				pos[i] = D3DXVECTOR3(x, 0.0f, z);
				i++;
			}
		}

		//�X�g���N�`���o�b�t�@����
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;				//�d�v
		bd.ByteWidth = sizeof(D3DXVECTOR3) * 1000000;
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
		srvd.Buffer.NumElements = 1000000;

		CRenderer::GetDevice()->CreateShaderResourceView(m_PositionBuffer, &srvd, &m_PositionSRV);

	}

	
	//m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//

}

void CGrass::Uninit()
{

	m_VertexBuffer->Release();
	m_Texture->Release();

	m_PositionBuffer->Release();

}

void CGrass::Update()
{
	CMeshField* meshField = CManager::GetScene()->GetGameObject<CMeshField>(1);

	m_Position.y = meshField->GetHeight(m_Position) + SIZE/2.0f;
}

void CGrass::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;
	//CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	//VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//vertex[0].Position = D3DXVECTOR3(-SIZE, SIZE, 0.0f);
	//vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	//
	//vertex[1].Position = D3DXVECTOR3(SIZE, SIZE, 0.0f);
	//vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[1].TexCoord = D3DXVECTOR2(1.0f , 0.0f);
	//
	//vertex[2].Position = D3DXVECTOR3(-SIZE, -SIZE, 0.0f);
	//vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	//
	//vertex[3].Position = D3DXVECTOR3(SIZE, -SIZE, 0.0f);
	//vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

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
	//shader_instancing->SetWorldMatrix(&world);
	

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//shader_instancing->SetMaterial(material);

	//�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�X�g���N�`���[�h�o�b�t�@�ݒ�
	CRenderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_PositionSRV);

	//�v���~�e�B�u�g�|���W�^
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�|���S���`��
	//CRenderer::GetDeviceContext()->Draw(4, 0);
	CRenderer::GetDeviceContext()->DrawInstanced(4, 10000, 0, 0);
}