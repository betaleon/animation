#pragma once
#include "lit.h"
#include "pop.h"
#include "game_object.h"

//class CAnimationModel;

class CPlayer : public CGameObject //åpè≥(ÉCÉìÉwÉäÉ^ÉìÉX)
{
private:
	
	//CModel* m_Model;
	CAnimationModel* m_AnimationModel;

	D3DXQUATERNION m_Quaternion;

	int m_Frame = 0;
	float m_BlendRate = 1.0f;
	CLit* shader_lit;
	CPop* shader_pop;

public:

	CPlayer(){}
	~CPlayer(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();

};