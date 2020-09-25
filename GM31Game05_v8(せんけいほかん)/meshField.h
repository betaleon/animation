#pragma once
#include "game_object.h"
#define TILE_WIDTH	10.0f
#define TILE_HEIGHT 10.0f
#define TILE_X 30
#define TILE_Z 30

class CMeshField : public CGameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11Buffer*				m_IndexBuffer = NULL;

	//VERTEX_3D m_vertex[(TILE_X + 1)*(TILE_Z+1)];
	VERTEX_3D m_vertex[TILE_X + 1][TILE_Z + 1];
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};