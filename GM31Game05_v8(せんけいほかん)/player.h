#pragma once
#include "lit.h"
#include "fog.h"
#include "pop.h"
#include "shadowMapping.h"
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
	CFog* shader_fog;
	CPop* shader_pop;
	CShadowM* shader_shadowM;

public:

	CPlayer(){}
	~CPlayer(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawPath1();

};