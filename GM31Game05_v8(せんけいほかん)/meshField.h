#pragma once
#include "lit.h"
#include "fog.h"
#include "pop.h"
#include "shadowMapping.h"
#include "game_object.h"
#define TILE_WIDTH	10.0f
#define TILE_HEIGHT 10.0f
#define TILE_X 81
#define TILE_Z 81

class CMeshField : public CGameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11Buffer*				m_IndexBuffer = NULL;

	//VERTEX_3D m_vertex[(TILE_X + 1)*(TILE_Z+1)];
	VERTEX_3D m_vertex[TILE_X + 1][TILE_Z + 1];

	CLit* shader_lit;
	CFog* shader_fog;
	CPop* shader_pop;
	CShadowM* shader_shadowM;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawPath1();
	float GetHeight(D3DXVECTOR3 Position);
	void DiamondSquare(int Array[TILE_X + 1][TILE_Z + 1], int size);
	void SquareStep(int Array[TILE_X + 1][TILE_Z + 1], int x, int z, int reach);
	void DiamondStep(int  Array[TILE_X + 1][TILE_Z + 1], int x, int z, int reach);
	float Random(int range);
};