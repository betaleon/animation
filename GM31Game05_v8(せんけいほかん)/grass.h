#pragma once

#include "game_object.h"


class CGrass : public CGameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer;
	ID3D11ShaderResourceView*	m_Texture;

public:


	void Init();
	void Uninit();
	void Update();
	void Draw();

};