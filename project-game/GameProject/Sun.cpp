/*==============================================================================
	
	Sun.cpp - 太陽
														Author : Yutaka Suganuma
														Date   : 2017/8/16
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Sun.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Light.h"

#include "InputKeyboard.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define DEFAULT_FIELD_SIZE (500.0f)

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* Sun::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Sun>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Sun::Sun( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//ライト
	GameObject *pLight = new GameObject( pGameObject);
	pLight->IsCreatedByOtherComponent = true;
	m_pLight = pLight->AddComponent<Light>();
	m_pLight->IsCreatedByOtherComponent = true;
	
	//ライトのカメラ
	m_pCamera = pLight->AddComponent<Camera>();
	m_pCamera->IsCreatedByOtherComponent = true;
	m_pCamera->ChangeLightCameraFormat( Vector3( DEFAULT_FIELD_SIZE, DEFAULT_FIELD_SIZE, DEFAULT_FIELD_SIZE));
	m_pCamera->SetPosAt( Vector3(0.0f, 0.0f, 0.0f));
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Sun::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void Sun::Update()
{
	//回転
	//Vector3 Axis = Vector3( 0.5f, 0.3f, 0.0f);
	//m_pTransform->RotateAxis( Axis, 0.01f);

	//移動
	/*Vector3 move;
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_RIGHT))
	{
		move.x += 1.0f;
	}
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_LEFT))
	{
		move.x -= 1.0f;
	}
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_UP))
	{
		move.z += 1.0f;
	}
	if (Manager::GetInputKeyboard()->GetKeyPress(DIK_DOWN))
	{
		move.z -= 1.0f;
	}

	m_pTransform->Move(move);*/
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void Sun::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void Sun::Save(Text& text)
{
	StartSave(text);

	

	EndSave( text);
}