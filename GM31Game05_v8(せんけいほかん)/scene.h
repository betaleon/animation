#pragma once
#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "game_object.h"
#include "camera.h"
#include "field.h"
#include "skydome.h"
#include "model.h"
#include "animation_model.h"
#include "player.h"
#include "polygon.h"
#include "enemy.h"
#include "bullet.h"

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
		
		AddGameObject<CField>(1);
		AddGameObject<CSkydome>(1);
		AddGameObject<CPlayer>(1);

		AddGameObject<CPolygon>(2);

		AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(-5.0f, 2.0f, 5.0f));
		AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 5.0f));
		AddGameObject<CEnemy>(1)->SetPosition(D3DXVECTOR3(5.0f, 2.0f, 5.0f));



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
			m_GameObject[i].clear();	//���X�g�̃N���A
		}
		CBullet::Unload();
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();	//���Ԑ��i�|�����[�t�B�Y���j
			}

			//list��������ɓ��Ă͂܂�v�f�����폜����
			m_GameObject[i].remove_if([](CGameObject* object) { return object->Destroy(); });	//�����_��
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

	//template�֐��͈����Ƃ��āu�^�v��n�����Ƃ��ł���
	//cpp�ŏ����Ă͂����Ȃ�
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
	
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*>objects;	//STL�̔z��

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