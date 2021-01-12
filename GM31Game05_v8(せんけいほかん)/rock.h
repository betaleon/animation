#pragma once
#include "fog.h"
#include "game_object.h"


class CRock : public CGameObject //継承(インヘリタンス)
{
private:

	class CModel* m_Model;
	//前方宣言(ポインタ変数の有効)
	CFog* shader_fog;

public:
	CRock() {}
	~CRock() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

};