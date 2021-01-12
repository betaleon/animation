#pragma once
#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "game_object.h"
#include "camera.h"
#include "field.h"
#include "meshField.h"
#include "skydome.h"
//#include "model.h"
#include "polygon.h"
#include "animation_model.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "grass.h"
#include "tree.h"
#include "rock.h"
#include "p_grass.h"

class CScene
{

protected:
	std::list<CGameObject*> m_GameObject[3];

public:
	CScene(){}
	virtual ~CScene(){}

	virtual void Init()
	{
		CBullet::Load();

		AddGameObject<CCamera>(0);
		
		//AddGameObject<CField>(1);
		AddGameObject<CMeshField>(1);
		AddGameObject<CSkydome>(1);

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

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();	//リストのクリア
		}
		CBullet::Unload();
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();	//多態性（ポリモーフィズム）
			}

			//listから条件に当てはまる要素だけ削除する
			m_GameObject[i].remove_if([](CGameObject* object) { return object->Destroy(); });	//ラムダ式
		}


	}

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}

	}

	//template関数は引数として「型」を渡すことができる
	//cppで書いてはいけない
	template <typename T>
	T* AddGameObject(int Layer)
	{

		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
		
	}

	template <typename T>
	T* GetGameObject(int Layer)
	{
	
		//T* gameObject = new T();
		//m_GameObject[Layer].push_back(gameObject);
		//gameObject->Init();
		//
		//return nullptr;

		for (CGameObject* object : m_GameObject[Layer]) {
			//typeid=型を調べる関数,#include <typeinfo>必要
			if (typeid(*object) == typeid(T))
			{
				return (T*)object;
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*>objects;	//STLの配列

		for (CGameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}

		return objects;
	}

};