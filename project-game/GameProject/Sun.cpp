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
	pLight->m_pTransform->SetLocalPosition( Vector3( 0.0f, 0.0f, 40.0f));
	pLight->m_pTransform->SetWorldRotationLookAt( Vector3( 0.0f, 0.0f, 0.0f));

	//ライトのカメラ
	m_pCamera = pLight->AddComponent<Camera>();
	m_pCamera->SetRenderTarget( "shadow", false);
	m_pCamera->SetRenderLayer( eLayerBack);
	m_pCamera->SetRenderLayer( eLayerDefault);
	m_pCamera->SetType( eCameraLight);

	m_pTransform->SetWorldPosition( -20.0f, 0.0f, -10.0f);
	m_pTransform->SetWorldRotationLookDirection( Vector3( -1.0f, 0.7f, -0.5f));
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
}
