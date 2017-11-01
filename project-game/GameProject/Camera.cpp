/*==============================================================================

    Camera.cpp - カメラ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "RenderTexture.h"
#include "ShaderManager.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define TEXTURE_WIDTH (1024)
#define TEXTURE_HEIGHT (1024)

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Camera::Camera( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_PosAt = Vector3();
	m_VecUp = Vector3( 0.0f, 1.0f, 0.0f);
	m_fFovY = D3DX_PI / 3.0f;
	m_fAspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	m_fNear = 0.1f;
	m_fFar = 100.0f;
	m_Type = eCameraDefault;

	////ビュー座標変換行列作成
	D3DXVECTOR3 PosEye = m_pTransform->GetWorldPosition().ConvertToDX();
	D3DXVECTOR3 PosAt = m_PosAt.ConvertToDX();
	D3DXVECTOR3 VecUp = m_VecUp.ConvertToDX();

	D3DXMatrixLookAtLH( &m_mtxView, &PosEye, &PosAt, &VecUp);

	//プロジェクション座標変換行列作成
	D3DXMatrixPerspectiveFovLH( 
		&m_mtxProj,								//プロジェクション座標変換行列ポインタ
		m_fFovY,								//画角（視野角）
		m_fAspect,								//アスペクト化
		m_fNear,								//near	※ 0.0f < near
		m_fFar);								//far

	//ビューポート設定
	m_ViewPort = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f};

	//レンダーターゲット（デフォルトはバックバッファ）
	m_RenderTarget = Manager::GetTextureManager()->CreateRenderTexture( "BuckBuffer", true);

	//描画するレイヤー（デフォルトはすべて描画）
	m_listRenderLayer.clear();

	//カメラをマネージャーに追加
	Manager::GetRenderManager()->SetCamera( this);
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Camera::Uninit( void)
{
	//マネージャーから削除
	Manager::GetRenderManager()->ReleaseCamera( this);
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void Camera::Update()
{
	////ビュー座標変換行列作成
	D3DXVECTOR3 PosEye = m_pTransform->GetWorldPosition().ConvertToDX();
	D3DXVECTOR3 PosAt = m_PosAt.ConvertToDX();
	D3DXVECTOR3 VecUp = m_VecUp.ConvertToDX();

	D3DXMatrixLookAtLH( &m_mtxView, &PosEye, &PosAt, &VecUp);
}

/*------------------------------------------------------------------------------
	カメラ方向を取得
------------------------------------------------------------------------------*/
Vector3 Camera::GetCameraVec( void) const
{
	 return ( m_PosAt - m_pTransform->GetWorldPosition()).Normalize();
}

/*------------------------------------------------------------------------------
	カメラ平行移動
------------------------------------------------------------------------------*/
void Camera::Move( const Vector3& Value)
{
	m_PosAt += Value;
	m_pTransform->Move( Value);
}

/*------------------------------------------------------------------------------
	レンダーターゲットの設定
------------------------------------------------------------------------------*/
void Camera::SetRenderTarget(std::string TextureName, bool isBuckBuffer)
{
	m_RenderTarget = Manager::GetTextureManager()->CreateRenderTexture( TextureName, isBuckBuffer);

	if (isBuckBuffer == false)
	{
		//プロジェクション座標変換
		m_fAspect = (float)TEXTURE_WIDTH / TEXTURE_HEIGHT;
		m_fNear = 8.0f;
		m_fFar = 100.0f;
		D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);

		//ビューポート変換
		SetViewPort( 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 1.0f);
	}
}

/*------------------------------------------------------------------------------
	カメラの設定（ビューポート、レンダーターゲット、深度バッファ）
------------------------------------------------------------------------------*/
void Camera::SetCamera(void)
{
	auto pDevice = Manager::GetDevice();

	//カメラの設定
	pDevice->SetViewport( &m_ViewPort);
	pDevice->SetRenderTarget( 0, m_RenderTarget->GetRenderTarget());
	pDevice->SetDepthStencilSurface( m_RenderTarget->GetDepthBuffer());

	//レンダーターゲットの初期化
	if (m_Type == eCameraLight)
	{
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 255, 255, 255, 255), 1.0f, 0);
		//pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 0, 0, 0, 255), 1.0f, 0);
	}
	else
	{
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 255, 255, 255, 255), 1.0f, 0);
	}

	//シェーダーの設定
	if (m_Type == eCameraDefault)
	{
		Manager::GetShaderManager()->SetDefault();
	}
	if (m_Type == eCameraLight)
	{
		Manager::GetShaderManager()->SetDepth();
	}
}

/*------------------------------------------------------------------------------
	ニアの設定
------------------------------------------------------------------------------*/
void Camera::SetNear(float Near)
{
	m_fNear = Near;
	D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
}

/*------------------------------------------------------------------------------
	ファーの設定
------------------------------------------------------------------------------*/
void Camera::SetFar(float Far)
{
	m_fFar = Far;
	D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
}
