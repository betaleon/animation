#pragma once

#include "game_object.h"


class CSkydome : public CGameObject //�p��(�C���w���^���X)
{
private:

	class CModel* m_Model;

public:
	CSkydome() {}
	~CSkydome() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

};