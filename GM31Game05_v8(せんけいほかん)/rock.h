#pragma once
#include "fog.h"
#include "game_object.h"


class CRock : public CGameObject //�p��(�C���w���^���X)
{
private:

	class CModel* m_Model;
	//�O���錾(�|�C���^�ϐ��̗L��)
	CFog* shader_fog;

public:
	CRock() {}
	~CRock() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

};