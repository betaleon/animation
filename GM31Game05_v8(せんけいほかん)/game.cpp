#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "camera.h"
#include "meshField.h"
#include "skydome.h"
#include "model.h"
#include "player.h"
#include "polygon.h"
#include "enemy.h"
#include "bullet.h"
#include "tree.h"
#include "grass.h"

void CGame::Init()
{
	for (int i = 0; i < 3; i++)
	{
		m_GameObject[i] = std::list<CGameObject*>();
	}

	CBullet::Load();

	AddGameObject<CCamera>(0);

	//AddGameObject<CField>(1);
	AddGameObject<CMeshField>(1);
	//AddGameObject<CSkydome>(1);

	AddGameObject<CPlayer>(1);
	//AddGameObject<CPolygon>(2);

	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(-5.0f, 2.0f, 5.0f));
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 5.0f));
	AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(5.0f, 2.0f, 5.0f));

	//Env-object
	AddGameObject<CTree>(1);
	//AddGameObject<CRock>(1);

	AddGameObject<CGrass>(1);

	//AddGameObject<CPgrass>(1);


}

void CGame::Uninit()
{
	CScene::Uninit();

	CBullet::Unload();
}

void CGame::Update()
{
	CScene::Update();

}