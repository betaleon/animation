#pragma once

#include "game_object.h"

//class CAnimationModel;

class CPlayer : public CGameObject //�p��(�C���w���^���X)
{
private:

	//CModel* m_Model;
	CAnimationModel* m_AnimationModel;

	D3DXQUATERNION m_Quaternion;

	int m_Frame = 0;
	float m_BlendRate = 1.0f;

public:

	CPlayer(){}
	~CPlayer(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();

};