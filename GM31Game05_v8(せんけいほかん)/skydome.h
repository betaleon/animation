#pragma once
#include "lit.h"
#include "game_object.h"


class CSkydome : public CGameObject //�p��(�C���w���^���X)
{
private:

	class CModel* m_Model;

	CLit* shader_lit;
public:
	CSkydome() {}
	~CSkydome() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

};