#pragma once

class CGameObject
{

protected:	//�A�N�Z�X�w��q
	bool			m_Destroy = false;

	D3DXVECTOR3		m_Position;
	D3DXVECTOR3		m_Rotation;
	D3DXVECTOR3		m_Scale;

public:		//�A�N�Z�X�w��q
	CGameObject(){}					//�R���X�g���N�^
	virtual ~CGameObject(){}		//�f�X�g���N�^(���z�֐�)

	virtual void Init() = 0;
	virtual void Uninit() = 0;		//�������z�֐�
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawPath1() = 0;

	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot,
			m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void SetRotation(D3DXVECTOR3 Rotation) { m_Rotation = Rotation; }
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }

	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
};