#include "main.h"
#include "renderer.h"
#include "MeshField.h"

//float g_FieldHeight[TILE_X][TILE_Z]=
//{
//	{0.0f,1.0f,.......},
//	{0.0f,1.0f,.......},
//	{0.0f,1.0f,.......},
//	{0.0f,1.0f,.......},
//};

void CMeshField::Init()
{
	
	
	//for (int i = 0, k = 0; i <= TILE_Z; i++)
	//{
	//	for (int j = 0; j <= TILE_X; j++)
	//	{
	//		m_vertex[k].Position = D3DXVECTOR3((-TILE_WIDTH * (TILE_X / 2.0f)) + j * TILE_WIDTH,  0 , (TILE_HEIGHT*(TILE_Z / 2.0f)) + (-i * TILE_HEIGHT));
	//		m_vertex[k].TexCoord = D3DXVECTOR2((1.0f*j) / (1.0f*TILE_X), (1.0f*i) / (1.0f*TILE_Z));
	//		k++;
	//	}
	//}
	//
	////�@���x�N�g���̐ݒ�
	//for (int i = 0; i < (TILE_X + 1)*(TILE_Z + 1); i++)
	//{
	//	m_vertex[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//}
	//
	////���ˌ��̐ݒ�(�F)
	//for (int i = 0; i < (TILE_X + 1)*(TILE_Z + 1); i++)
	//{
	//	m_vertex[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//}

	for (int x = 0; x <= TILE_X; x++)
	{
		for (int z = 0; z <= TILE_Z; z++)
		{
			//float y = sinf(x*0.5f)*5.0f;
			float y = sinf(x*0.5f)*sinf(z*0.5f)*5.0f;
			//float y = g_FieldHeight[z][x];
			m_vertex[x][z].Position = D3DXVECTOR3((x - 10)*5.0f,y, (z - 10)*-5.0f);
			m_vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_vertex[x][z].TexCoord = D3DXVECTOR2(x*0.5f, z*0.5f);
		}
	}

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * (TILE_X + 1)*(TILE_Z + 1);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�C���f�b�N�X�o�b�t�@����
	{
		//unsigned int index[((4 + (TILE_X - 1) * 2)*TILE_Z + 2 * (TILE_Z - 1))*2];
		unsigned int index[((TILE_X + 2) * 2) * TILE_Z - 2];
		//�C���f�b�N�X�f�[�^���o�b�t�@�֊i�[
		//for (int i = 0, j = 0, k = (TILE_X + 1), s = 0; i < ((4 + (TILE_X - 1) * 2)*TILE_Z + 2 * (TILE_Z - 1)); i++)
		//{
		//
		//	if (i % 2 == 0)
		//	{
		//		index[i + s] = k;
		//		k++;											//k�̓C���f�b�N�X�o�b�t�@�̑傫���l�̂ق�
		//	}
		//	else
		//	{
		//		index[i + s] = j;
		//		j++;											//j�̓C���f�b�N�X�o�b�t�@�̏������l�̂ق�
		//	}
		//
		//	if ((i + 1) % ((TILE_X + 1) * 2) == 0)				//�k�ރ|���S���̊i�[
		//	{
		//		index[i + s + 1] = j - 1;
		//		index[i + s + 2] = k;
		//		s += 2;											//s�͏k�ރ|���S���̃J�E���^
		//	}
		//}

		int i = 0;
		for (int x = 0; x < TILE_X; x++)
		{
			for (int z = 0; z < TILE_Z + 1; z++)
			{
				index[i] = x * (TILE_X + 1) + z;
				i++;

				index[i] = (x + 1) * (TILE_X + 1) + z;
				i++;
			}
			if (x == (TILE_X-1))
				break;
			index[i] = (x + 1) * (TILE_X + 1) + TILE_Z;
			i++;

			index[i] = (x + 1)*(TILE_X + 1);
			i++;
		}

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(unsigned int) * ((4 + (TILE_X - 1) * 2)*TILE_Z + 2 * (TILE_Z - 1));
		bd.ByteWidth = sizeof(unsigned int) * ((TILE_X + 2) * 2) * TILE_Z - 2;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);

	}


	//�e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/grass.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

}

void CMeshField::Uninit()
{

	m_VertexBuffer->Release();
	m_Texture->Release();

}

void CMeshField::Update()
{


}

void CMeshField::Draw()
{
	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	CRenderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�^
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	//CRenderer::GetDeviceContext()->DrawIndexed((4 + ((TILE_X - 1) * 2)*TILE_Z + 2 * (TILE_Z - 1)), 0, 0);
	CRenderer::GetDeviceContext()->DrawIndexed(((TILE_X + 2) * 2) * TILE_Z - 2, 0, 0);

}