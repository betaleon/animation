#pragma once
#include "lit.h"
#include "game_object.h"

class CCamera:public CGameObject
{

private:
	D3DXVECTOR3 m_Target;

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	CLit* shader_lit;

public:
	CCamera(){}
	~CCamera(){}
	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXMATRIX GetViewMatrix() { return m_viewMatrix; }
};