#pragma once
#include "lit.h"
#include "game_object.h"


class CEnemy : public CGameObject //�p��(�C���w���^���X)
{
private:

	class CModel* m_Model;
	//�O���錾(�|�C���^�ϐ��̗L��)
	CLit* shader_lit;

public:
	CEnemy() {}
	~CEnemy() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawPath1() {};

};