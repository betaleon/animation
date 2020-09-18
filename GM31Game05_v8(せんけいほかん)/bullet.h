#pragma once

#include "game_object.h"


class CBullet : public CGameObject //Œp³(ƒCƒ“ƒwƒŠƒ^ƒ“ƒX)
{
private:

	static class CModel* m_Model;

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