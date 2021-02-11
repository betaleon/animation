#pragma once
#include "scene.h"
class CPlayer;

class CManager
{

	static class CScene* m_Scene;

public:

	//static class CScene* m_Scene;
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static class CScene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		T* scene = new T();

		m_Scene = scene;

		m_Scene->Init();

	}

};