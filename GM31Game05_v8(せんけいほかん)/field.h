#pragma once
#include "lit.h"
#include "game_object.h"


class CField : public CGameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	CLit* shader_lit;

public:
	CField() {}
	~CField(){}
	void Init();
	void Uninit();
	void Update();
	void Draw();

};