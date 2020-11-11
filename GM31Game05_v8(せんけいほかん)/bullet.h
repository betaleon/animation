#pragma once

#include "lit.h"
#include "game_object.h"


class CBullet : public CGameObject //�p��(�C���w���^���X)
{
private:

	static class CModel* m_Model;

	CLit* shader_lit;

public:
	CBullet() {}
	~CBullet() {}

	static void Load();
	static void Unload();


	void Init();
	void Uninit();
	void Update();
	void Draw();

};