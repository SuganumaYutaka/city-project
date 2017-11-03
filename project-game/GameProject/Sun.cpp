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
	コンストラクタ
------------------------------------------------------------------------------*/
Sun::Sun( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//ライト
	GameObject *pLight = new GameObject( pGameObject);
	m_pLight = pLight->AddComponent<Light>();
	pLight->m_pTransform->SetLocalPosition( Vector3( 0.0f, 0.0f, 0.0f));
	
	//ライトのカメラ
	m_pCamera = pLight->AddComponent<Camera>();
	m_pCamera->SetRenderTarget( "shadow", false);
	m_pCamera->SetRenderLayer( eLayerBack);
	m_pCamera->SetRenderLayer( eLayerDefault);
	m_pCamera->SetType( eCameraLight);

	// Orthoの設定
	float length = 1500.0f * 1.4142f;
	m_pCamera->SetOrtho( length, length, 0.1f, length);

	m_pTransform->SetWorldPosition( -750.0f, 600.0f, -300.0f);
	m_pTransform->SetWorldRotationLookAt( Vector3(750.0f, 0.0f, 750.0f));
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

