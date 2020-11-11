#pragma once

#include "lit.h"
#include "game_object.h"


class CGrass : public CGameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer;
	ID3D11ShaderResourceView*	m_Texture;

	CLit* shader_lit;

public:


	void Init();
	void Uninit();
	void Update();
	void Draw();

};