#include "main.h"
#include "renderer.h"
#include "MeshField.h"
#include "animation_model.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include <time.h>


//float g_FieldHeight[TILE_X][TILE_Z]=
//{
//	{0.0f,1.0f,.......},
//	{0.0f,1.0f,.......},
//	{0.0f,1.0f,.......},
//	{0.0f,1.0f,.......},
//};

void CMeshField::Init()
{
	{
		//shader_pop = (CPop*)CRenderer::GetShader();
		//shader_fog = CRenderer::GetShader<CFog>();
		shader_shadowM = CRenderer::GetShader<CShadowM>();
	}

	int Array[TILE_X + 1][TILE_Z + 1] = {};
	DiamondSquare(Array, TILE_X);

	for (int x = 0; x <= TILE_X; x++)
	{
		for (int z = 0; z <= TILE_Z; z++)
		{
			//float y = sinf(x*0.5f)*5.0f;
			float y = sinf(x*0.5f)*sinf(z*0.5f)*5.0f + Array[x][z];
			//float y =  Array[x][z];
			//float y = g_FieldHeight[z][x];
			m_vertex[x][z].Position = D3DXVECTOR3((x - 10)*5.0f, y, (z - 10)*-5.0f);
			m_vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_vertex[x][z].TexCoord = D3DXVECTOR2(x*0.5f, z*0.5f);
		}
	}
		
	m_vertex[0][0].TexCoord = D3DXVECTOR2(0, 0);
	//法線ベクトル算出
	for (int x = 1; x <= TILE_X-1; x++)
	{
		for (int z = 1; z <= TILE_Z-1; z++)
		{
			D3DXVECTOR3 vx, vz, vn;

			vx = m_vertex[x + 1][z].Position - m_vertex[x - 1][z].Position;
			vz = m_vertex[x][z - 1].Position - m_vertex[x][z + 1].Position;

			D3DXVec3Cross(&vn, &vz, &vx);	//外積
			D3DXVec3Normalize(&vn, &vn);	//正規化(長さ１にする)
			m_vertex[x][z].Normal = vn;
		}
	}

	//頂点バッファ生成
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

	//インデックスバッファ生成
	{
		//unsigned int index[((4 + (TILE_X - 1) * 2)*TILE_Z + 2 * (TILE_Z - 1))*2];
		unsigned int index[((TILE_X + 2) * 2) * TILE_Z - 2];
		//インデックスデータをバッファへ格納
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


	//テクスチャ読み込み
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
	//shader_shadowM->UpdateConstantBuffers();
	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	{
		//shader
		//shader_lit->SetWorldMatrix(&world);
		//shader_fog->SetWorldMatrix(&world);
		//shader_pop->SetWorldMatrix(&world);
		shader_shadowM->SetWorldMatrix(&world);
	}

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックスバッファの設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	{
		//shader
		//shader_lit->SetMaterial(material);
		//shader_fog->SetMaterial(material);
		//shader_pop->SetMaterial(material);
		shader_shadowM->SetMaterial(material);
	}

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//シャドウバッファをセット
	ID3D11ShaderResourceView* shadowDepthTexture = CRenderer::GetShadowDepthTexture();//-追加
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);//-追加

	//プリミティブトポロジ型
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::SetShader(shader_shadowM);

	//ポリゴン描画
	//CRenderer::GetDeviceContext()->DrawIndexed((4 + ((TILE_X - 1) * 2)*TILE_Z + 2 * (TILE_Z - 1)), 0, 0);
	//CRenderer::SetDepthEnable(TRUE);
	CRenderer::GetDeviceContext()->DrawIndexed(((TILE_X + 2) * 2) * TILE_Z - 2, 0, 0);

}

void CMeshField::DrawPath1()
{
	//shader_shadowM->UpdateConstantBuffers();
	//SetShader
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	CPlayer* pPlayer = CManager::GetScene()->GetGameObject<CPlayer>(1);
	//-----------ライトをカメラとみなした行列を作成
	D3DXMatrixLookAtLH(&light.ViewMatrix, &(D3DXVECTOR3(-10.0f, 20.0f, -10.0f) + pPlayer->GetPosition()),
		&pPlayer->GetPosition(), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//-----------ライト用のプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 100.0f);
	shader_shadowM->SetLight(light);

	//shader_shadowM->BeginDepth();
	CRenderer::BeginDepth();

	shader_shadowM->SetViewMatrix(&light.ViewMatrix);
	shader_shadowM->SetProjectionMatrix(&light.ProjectionMatrix);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	{
		//shader
		shader_shadowM->SetWorldMatrix(&world);
	}

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックスバッファの設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	{
		//shader
		shader_shadowM->SetMaterial(material);
	}

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	//シャドウバッファをセット
	ID3D11ShaderResourceView* shadowDepthTexture = CRenderer::GetShadowDepthTexture();//-追加
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);//-追加

	//プリミティブトポロジ型
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::SetShader(shader_shadowM);

	//ポリゴン描画
	//CRenderer::GetDeviceContext()->DrawIndexed((4 + ((TILE_X - 1) * 2)*TILE_Z + 2 * (TILE_Z - 1)), 0, 0);
	//CRenderer::SetDepthEnable(TRUE);
	CRenderer::GetDeviceContext()->DrawIndexed(((TILE_X + 2) * 2) * TILE_Z - 2, 0, 0);

}

float CMeshField::GetHeight(D3DXVECTOR3 Position)
{
	int x, z;

	//ブロック番号算出
	x = Position.x / 5.0f + 10.0f;
	z = Position.z / -5.0f + 10.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = m_vertex[x][z].Position;
	pos1 = m_vertex[x + 1][z].Position;
	pos2 = m_vertex[x][z + 1].Position;
	pos3 = m_vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;
	v1p = Position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;

	if (c.y > 0.0f)
	{
		//左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		//右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//高さ取得
	py = -((Position.x - pos1.x)*n.x + (Position.z - pos1.z)*n.z) / n.y + pos1.y;

	return py;
}

void CMeshField::DiamondSquare(int Array[TILE_X + 1][TILE_Z + 1], int size)
{
	int half = size /2;
	if (half < 1)
		return;
	//square steps
	for (int z = half; z < TILE_Z; z += size)
		for (int x = half; x < TILE_X; x += size)
			SquareStep(Array, x % TILE_X, z % TILE_Z, half);
	// diamond steps
	int col = 0;
	for (int x = 0; x < TILE_X; x += half)
	{
		col++;
		//If this is an odd column.
		if (col % 2 == 1)
			for (int z = half; z < TILE_Z; z += size)
				DiamondStep(Array, x % TILE_X, z % TILE_Z, half);
		else
			for (int z = 0; z < TILE_Z; z += size)
				DiamondStep(Array, x % TILE_X, z % TILE_Z, half);
	}
	DiamondSquare(Array, size / 2);
}

void CMeshField::SquareStep(int Array[TILE_X + 1][TILE_Z + 1], int x, int z, int reach)
{
	int count = 0;
	float avg = 0.1f;
	if (x - reach >= 0 && z - reach >= 0)
	{
		avg += Array[x - reach][z - reach];
		count++;
	}
	if (x - reach >= 0 && z + reach < TILE_Z)
	{
		avg += Array[x - reach][z + reach];
		count++;
	}
	if (x + reach < TILE_X && z - reach >= 0)
	{
		avg += Array[x + reach][z - reach];
		count++;
	}
	if (x + reach < TILE_X && z + reach < TILE_Z)
	{
		avg += Array[x + reach][z + reach];
		count++;
	}
	avg += Random(reach);
	avg /= count;
	Array[x][z] = round(avg);
}

void CMeshField::DiamondStep(int Array[TILE_X + 1][TILE_Z + 1], int x, int z, int reach)
{
	int count = 0;
	float avg = 5.0f;
	if (x - reach >= 0)
	{
		m_avg += Array[x - reach][z];
		count++;
	}
	if (x + reach < TILE_X)
	{
		m_avg += Array[x + reach][z];
		count++;
	}
	if (z - reach >= 0)
	{
		m_avg += Array[x][z - reach];
		count++;
	}
	if (z + reach < TILE_Z)
	{
		m_avg += Array[x][z + reach];
		count++;
	}
	m_avg += Random(reach);
	m_avg /= count;
	Array[x][z] = (int)m_avg;
}

float CMeshField::Random(int range)
{
	srand(time(NULL));

	return fabs((rand() % (range * 2)) - range);
}