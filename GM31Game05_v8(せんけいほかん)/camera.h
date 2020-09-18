#pragma once
#include "game_object.h"

class CCamera:public CGameObject
{

private:
	D3DXVECTOR3 m_Target;

public:
	CCamera(){}
	~CCamera(){}
	void Init();
	void Uninit();
	void Update();
	void Draw();

};