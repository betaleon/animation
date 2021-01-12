#pragma once

#include "lit.h"
#include "instancing.h"
#include "game_object.h"

#define TILE_WIDTH	10.0f
#define TILE_HEIGHT 10.0f
#define TILE_X 30
#define TILE_Z 30


class CPgrass : public CGameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer;
	ID3D11ShaderResourceView*	m_Texture;

	ID3D11Buffer*				m_PositionBuffer;
	ID3D11ShaderResourceView*	m_PositionSRV;

	CInstancing* shader_instancing;

public:


	void Init();
	void Uninit();
	void Update();
	void Draw();

};