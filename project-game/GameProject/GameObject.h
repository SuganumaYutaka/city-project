/*==============================================================================
	
	GameObject.h - �I�u�W�F�N�g�m�[�h
														Author : Yutaka Suganuma
														Date   : 2017/4/29
==============================================================================*/
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

#include "Component.h"
#include "Transform.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Transform;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class GameObject
{
public:
	GameObject( GameObject* pParent);
	
	void AddChild( GameObject* pChild);
	void UpdateAll();
	void DrawAll();
	
	void OnCollision( Collider *pCollider);			//�Փ˂����Ƃ��Ăяo�����
	
	GameObject* GetParent() { return m_pParent;}
	GameObject* GetRoot() { return m_pRoot;}
	
	Transform* m_pTransform;						//�g�����X�t�H�[���R���|�[�l���g
	std::string m_Tag;								//�^�O

	void ReleaseReserve( void );
	static void ReleaseList( void);

	void ReleaseComponent( Component* pComp);

	void Save( Text& text);
	void Load( Text& text);

	bool IsCreatedByOtherComponent;

	void SetImGuiView();

	bool IsSelected;
	
private:
	GameObject();
	~GameObject() {}

	GameObject* m_pParent;							//�e�m�[�h
	GameObject* m_pRoot;							//���[�g�m�[�h
	std::list< GameObject*> m_listChild;			//�q�m�[�h���X�g
	std::list< Component*> m_listComponent;			//�R���|�[�l���g���X�g
	
	void InitGameObject( GameObject* pParent);
	void Update();
	void Draw();
	void ReleaseReserveChildren( void );
	void ReleaseObject();
	void ReleaseRecursive();
	
	static std::list<GameObject*> m_listReleaseObject;	//�������X�g
	bool m_bRelease;									//�����\��ς݂�

public:
	template< class T> T* GetComponent(void)	//�R���|�[�l���g�擾
	{
		//�R���|�[�l���g�̌���
		for (Component* pComp : m_listComponent)
		{
			if( typeid(*pComp) == typeid(T))
			{
				return (T *)pComp;
			}
		}

		return NULL;
	}

	template< class T> std::list<T*> GetComponentList(void)	//�R���|�[�l���g�擾
	{
		std::list<T*> list;

		//�R���|�[�l���g�̌���
		for (Component* pComp : m_listComponent)
		{
			if( typeid(*pComp) == typeid(T))
			{
				list.push_back( (T *)pComp);
			}
		}

		return list;
	}

	template< class T> T* GetComponentInChildren(void)	//�q�̃R���|�[�l���g�擾
	{
		//�R���|�[�l���g�̌���
		for( GameObject* pObj : m_listChild)
		{
			for (Component* pComp : pObj->m_listComponent)
			{
				if( typeid(*pComp) == typeid(T))
				{
					return (T *)pComp;
				}
			}
		}

		return NULL;
	}

	template< class T> std::list<T*> GetComponentListInChildren(void)	//�q�̃R���|�[�l���g���X�g�擾
	{
		std::list<T*> list;

		//�R���|�[�l���g�̌���
		for( GameObject* pObj : m_listChild)
		{
			for (Component* pComp : pObj->m_listComponent)
			{
				if( typeid(*pComp) == typeid(T))
				{
					list.push_back( (T *)pComp);
				}
			}
		}

		return list;
	}


	template< class T> T* AddComponent(void)	//�R���|�[�l���g�ǉ�
	{
		//�R���|�[�l���g�ǉ�
		T* pComp = new T( this);
		m_listComponent.push_back( pComp);
		return pComp;
	}
};



#endif

