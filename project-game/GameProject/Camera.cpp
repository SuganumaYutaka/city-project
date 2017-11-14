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
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* Camera::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Camera>();
}

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

/*------------------------------------------------------------------------------
	プロジェクション行列の設定（Perspective）
------------------------------------------------------------------------------*/
void Camera::SetPerspective( float FovY, float Aspect, float Near, float Far)
{
	m_fFovY = FovY;
	m_fAspect = Aspect;
	m_fNear = Near;
	m_fFar = Far;

	D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
}

/*------------------------------------------------------------------------------
	プロジェクション行列の設定（Perspective）
------------------------------------------------------------------------------*/
void Camera::SetOrtho( float Width, float Height, float Near, float Far)
{
	m_fNear = Near;
	m_fFar = Far;
	D3DXMatrixOrthoLH( &m_mtxProj, Width, Height, Near, Far);
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void Camera::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "PosAt")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_PosAt.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		else if (text.GetWord() == "VecUp")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_VecUp.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		else if (text.GetWord() == "FovY")
		{
			text.ForwardPositionToNextWord();
			m_fFovY = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Aspect")
		{
			text.ForwardPositionToNextWord();
			m_fAspect = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Near")
		{
			text.ForwardPositionToNextWord();
			m_fNear = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Far")
		{
			text.ForwardPositionToNextWord();
			m_fFar = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "RenderLayer")
		{
			text.ForwardPositionToNextWord();
			int size = std::stoi(text.GetWord());
			for( int nCnt = 0; nCnt < size; nCnt++)
			{
				text.ForwardPositionToNextWord();
				m_listRenderLayer.push_back(std::stoi(text.GetWord()));
			}
		}

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

	//ビュー座標変換行列作成
	D3DXVECTOR3 PosEye = m_pTransform->GetWorldPosition().ConvertToDX();
	D3DXVECTOR3 PosAt = m_PosAt.ConvertToDX();
	D3DXVECTOR3 VecUp = m_VecUp.ConvertToDX();
	D3DXMatrixLookAtLH( &m_mtxView, &PosEye, &PosAt, &VecUp);

	//プロジェクション座標変換行列作成
	SetPerspective( m_fFovY, m_fAspect, m_fNear, m_fFar);
}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void Camera::Save(Text& text)
{
	StartSave(text);

	text += "PosAt " + m_PosAt.ConvertToString() + '\n';
	text += "VecUp " + m_VecUp.ConvertToString() + '\n';
	text += "FovY " + std::to_string(m_fFovY) + '\n';
	text += "Aspect " + std::to_string(m_fAspect) + '\n';
	text += "Near " + std::to_string(m_fNear) + '\n';
	text += "Far " + std::to_string(m_fFar) + '\n';

	int size = m_listRenderLayer.size();
	if( size > 0)
	{
		text += "RenderLayer " + std::to_string(size) + '\n';
		for( auto ite = m_listRenderLayer.begin(); ite != m_listRenderLayer.end(); ++ite)
		{
			text += std::to_string(*ite) + '\n';
		}
	}

	EndSave( text);
}