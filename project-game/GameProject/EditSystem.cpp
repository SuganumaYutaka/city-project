/*==============================================================================
	
	EditSystem.cpp - エディットシステム
														Author : Yutaka Suganuma
														Date   : 2017/11/17
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "EditSystem.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Light.h"

#include "InputMouse.h"
#include "CollisionManager.h"

/*------------------------------------------------------------------------------
	静的メンバ変数宣言
------------------------------------------------------------------------------*/
EditSystem* EditSystem::Instance = NULL;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* EditSystem::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<EditSystem>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
EditSystem::EditSystem( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	SelectedGameObject = NULL;

	if (Instance != NULL)
	{
		ReleaseReserve();
		return;
	}
	Instance = this;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void EditSystem::Uninit( void)
{
	Instance = NULL;
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void EditSystem::Update()
{
	if (ImGui::IsAnyWindowHovered())
	{
		return;
	}

	//オブジェクトを選択
	if (Manager::GetInputMouse()->GetClickTrigger(0))
	{
		//レイの取得
		auto ray = Camera::GetMainCamera()->CalcScreenPointToRay(Manager::GetInputMouse()->GetScreenPosition());
		RaycastHit hit;
		if (Manager::GetCollisionManager()->Raycast(ray, -1.0f, &hit))
		{
			if( SelectedGameObject)
			{
				SelectedGameObject->IsSelected = false;
			}
			SelectedGameObject = hit.pGameObject;
			SelectedGameObject->IsSelected = true;
		}
		else
		{
			if( SelectedGameObject)
			{
				SelectedGameObject->IsSelected = false;
				SelectedGameObject = NULL;
			}
		}
	}
}
