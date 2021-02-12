
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "debugimgui.h"
#include "imgui.h"
#include "animation_model.h"
#include "player.h"
#include "meshField.h"
#include "game.h"

CScene* CManager::m_Scene = nullptr;

//CShadowM* g_shadowM;

void CManager::Init()
{

	CRenderer::Init();
	CDebugGui::Init();
	CInput::Init();

	//m_Scene = new CScene();
	//m_Scene->Init();
	SetScene<CGame>();

}

void CManager::Uninit()
{

	m_Scene->Uninit();
	delete m_Scene;

	CInput::Uninit();
	CDebugGui::Finalize();
	CRenderer::Uninit();
}

void CManager::Update()
{
	CInput::Update();
	CDebugGui::Update();

	m_Scene->Update();
}

void CManager::Draw()
{


	m_Scene->DrawPath1();

	CDebugGui::Begin();
	CRenderer::Begin();

	m_Scene->Draw();

	//light.Enable = false;



	//ImGui
	ImGui::SetNextWindowPos(ImVec2(20, 20));

	ImGui::Begin("Debug");
	
	CPlayer* pPlayer = m_Scene->GetGameObject<CPlayer>(1);
	if (pPlayer != NULL)
	{
		ImGui::Text("PlayerSize");
		static float a = 0.01f;
		pPlayer->SetScale(D3DXVECTOR3(a, a, a));
		ImGui::SliderFloat("a", &a, 0.00f, 0.02f);
	}

	/*
	CMeshField* pMeshfield = m_Scene->GetGameObject<CMeshField>(1);
	if (pMeshfield != NULL)
	{
		ImGui::Text("MeshFieldAvg");
		static float avg = 1.0f;
		pMeshfield->m_avg = avg;
		ImGui::SliderFloat("stagescale", &avg, 10.00f, 0.02f);

		{
			//shader_pop = (CPop*)CRenderer::GetShader();
			//shader_fog = CRenderer::GetShader<CFog>();
			pMeshfield->shader_shadowM = CRenderer::GetShader<CShadowM>();
		}

		int Array[TILE_X + 1][TILE_Z + 1] = {};
		pMeshfield->DiamondSquare(Array, TILE_X);

		for (int x = 0; x <= TILE_X; x++)
		{
			for (int z = 0; z <= TILE_Z; z++)
			{
				//float y = sinf(x*0.5f)*5.0f;
				//float y = sinf(x*0.5f)*sinf(z*0.5f)*5.0f + Array[x][z];
				float y = Array[x][z];
				//float y = g_FieldHeight[z][x];
				pMeshfield->m_vertex[x][z].Position = D3DXVECTOR3((x - 10)*5.0f, y, (z - 10)*-5.0f);
				pMeshfield->m_vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pMeshfield->m_vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				pMeshfield->m_vertex[x][z].TexCoord = D3DXVECTOR2(x*0.5f, z*0.5f);
			}
		}

		pMeshfield->m_vertex[0][0].TexCoord = D3DXVECTOR2(0, 0);
		//法線ベクトル算出
		for (int x = 1; x <= TILE_X - 1; x++)
		{
			for (int z = 1; z <= TILE_Z - 1; z++)
			{
				D3DXVECTOR3 vx, vz, vn;

				vx = pMeshfield->m_vertex[x + 1][z].Position - pMeshfield->m_vertex[x - 1][z].Position;
				vz = pMeshfield->m_vertex[x][z - 1].Position - pMeshfield->m_vertex[x][z + 1].Position;

				D3DXVec3Cross(&vn, &vz, &vx);	//外積
				D3DXVec3Normalize(&vn, &vn);	//正規化(長さ１にする)
				pMeshfield->m_vertex[x][z].Normal = vn;
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
		sd.pSysMem = pMeshfield->m_vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pMeshfield->m_VertexBuffer);

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
				if (x == (TILE_X - 1))
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
			CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &pMeshfield->m_IndexBuffer);

		}


		//テクスチャ読み込み
		D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
			"asset/texture/grass.jpg",
			NULL,
			NULL,
			&pMeshfield->m_Texture,
			NULL);

		assert(m_Texture);
	}
	*/
	
	
	ImGui::End();

	CDebugGui::End();
	CRenderer::End();
}
