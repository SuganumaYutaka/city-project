/*==============================================================================
	
	GameObject.h - オブジェクトノード
														Author : Yutaka Suganuma
														Date   : 2017/4/29
==============================================================================*/
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

#include "Component.h"
#include "Transform.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Transform;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class GameObject
{
public:
	GameObject( GameObject* pParent);
	
	void AddChild( GameObject* pChild);
	void UpdateAll();
	void DrawAll();
	
	void OnCollision( Collider *pCollider);			//衝突したとき呼び出される
	
	GameObject* GetParent() { return m_pParent;}
	GameObject* GetRoot() { return m_pRoot;}
	
	Transform* m_pTransform;						//トランスフォームコンポーネント
	std::string m_Tag;								//タグ

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

	GameObject* m_pParent;							//親ノード
	GameObject* m_pRoot;							//ルートノード
	std::list< GameObject*> m_listChild;			//子ノードリスト
	std::list< Component*> m_listComponent;			//コンポーネントリスト
	
	void InitGameObject( GameObject* pParent);
	void Update();
	void Draw();
	void ReleaseReserveChildren( void );
	void ReleaseObject();
	void ReleaseRecursive();
	
	static std::list<GameObject*> m_listReleaseObject;	//消去リスト
	bool m_bRelease;									//消去予約済みか

public:
	template< class T> T* GetComponent(void)	//コンポーネント取得
	{
		//コンポーネントの検索
		for (Component* pComp : m_listComponent)
		{
			if( typeid(*pComp) == typeid(T))
			{
				return (T *)pComp;
			}
		}

		return NULL;
	}

	template< class T> std::list<T*> GetComponentList(void)	//コンポーネント取得
	{
		std::list<T*> list;

		//コンポーネントの検索
		for (Component* pComp : m_listComponent)
		{
			if( typeid(*pComp) == typeid(T))
			{
				list.push_back( (T *)pComp);
			}
		}

		return list;
	}

	template< class T> T* GetComponentInChildren(void)	//子のコンポーネント取得
	{
		//コンポーネントの検索
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

	template< class T> std::list<T*> GetComponentListInChildren(void)	//子のコンポーネントリスト取得
	{
		std::list<T*> list;

		//コンポーネントの検索
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


	template< class T> T* AddComponent(void)	//コンポーネント追加
	{
		//コンポーネント追加
		T* pComp = new T( this);
		m_listComponent.push_back( pComp);
		return pComp;
	}
};



#endif

