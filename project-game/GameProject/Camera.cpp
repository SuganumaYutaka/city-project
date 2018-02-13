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
	メインカメラを取得
------------------------------------------------------------------------------*/
Camera* Camera::GetMainCamera()
{
	return Manager::GetRenderManager()->GetMainCamera();
}

/*------------------------------------------------------------------------------
	メインカメラを設定
------------------------------------------------------------------------------*/
void Camera::SetMainCamera()
{
	Manager::GetRenderManager()->SetMainCamera( this);
}

/*------------------------------------------------------------------------------
	メインカメラかどうか
------------------------------------------------------------------------------*/
bool Camera::IsMainCamera()
{
	if ( this == Manager::GetRenderManager()->GetMainCamera())
	{
		return true;
	}

	return false;
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
		auto size = m_RenderTarget->GetTextureSize();

		//プロジェクション座標変換
		m_fAspect = size.x / size.y;
		D3DXMatrixPerspectiveFovLH( &m_mtxProj, m_fFovY, m_fAspect, m_fNear, m_fFar);

		//ビューポート変換
		SetViewPort( 0, 0, (int)size.x, (int)size.y, 0.0f, 1.0f);
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
	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 255, 255, 255, 255), 1.0f, 0);
	//pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA( 0, 0, 0, 0), 1.0f, 0);

	//シェーダーの設定
	if (m_Type == eCameraDefault)
	{
		Manager::GetShaderManager()->SetDefault();
	}
	else if (m_Type == eCameraLight)
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
	スクリーン座標からレイ（直線）を算出
------------------------------------------------------------------------------*/
Ray Camera::CalcScreenPointToRay(Vector2 screenPoint)
{
	//スクリーン座標からワールド座標に変換
	Vector3 nearPos = CalcScreenPointToWorldPosition( screenPoint, 0.0f);		//カメラから最も近い座標
	Vector3 farPos = CalcScreenPointToWorldPosition( screenPoint, 1.0f);		//カメラから最も遠い座標

	//カーソルの線分算出
	Ray ray;
	ray.Direction = (farPos - nearPos).Normalize();		//カーソルの線分
	ray.Position = m_pTransform->GetWorldPosition();

	return ray;
}

/*------------------------------------------------------------------------------
	スクリーン座標からワールド座標を算出
------------------------------------------------------------------------------*/
Vector3 Camera::CalcScreenPointToWorldPosition( Vector2 screenPoint, float positionZ)
{
	//変数宣言
	D3DXMATRIX mtxWorld;				//ワールド座標変換行列
	D3DXVECTOR3 worldPos;				//ワールド座標

	//逆行列を設定
	D3DXMATRIX mtxViewInv;		//ビュー座標変換行列の逆行列
	D3DXMATRIX mtxProjInv;		//プロジェクション座標変換行列の逆行列
	D3DXMatrixInverse( &mtxViewInv, NULL, &m_mtxView);
	D3DXMatrixInverse( &mtxProjInv, NULL, &m_mtxProj);
	
	//ビューポート（拾い物）
	D3DXMATRIX VP, InvViewport;
	D3DXMatrixIdentity( &VP );
	VP._11 = SCREEN_WIDTH / 2.0f;
	VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f;
	VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse( &InvViewport, NULL, &VP );

	//逆行列をかけていきワールド座標変換行列に変換
	D3DXMatrixIdentity( &mtxWorld);		//単位行列化
	mtxWorld = mtxWorld * InvViewport * mtxProjInv * mtxViewInv;
	
	//ワールド座標を算出
	D3DXVec3TransformCoord( &worldPos, &D3DXVECTOR3( screenPoint.x, screenPoint.y, positionZ), &mtxWorld);

	return worldPos;
}

/*------------------------------------------------------------------------------
	ライトカメラ用の設定
------------------------------------------------------------------------------*/
bool Camera::ChangeLightCameraFormat(Vector3 fieldSize)
{
	//カメラの種類の設定
	SetType( eCameraLight);

	//描画レイヤーの設定
	SetRenderLayer( eLayerBack);
	SetRenderLayer( eLayerDefault);
	
	//レンダーターゲットの設定
	SetRenderTarget( "shadow", false);
	m_RenderTarget->ChangeDepthRenderFormat();

	//Orthoの設定
	float length = max( fieldSize.x, fieldSize.z);
	SetOrtho( length, length, 0.1f, length);

	//ビューポートの設定
	auto textureSize = m_RenderTarget->GetTextureSize();
	SetViewPort( 0, 0, (int)textureSize.x, (int)textureSize.y, 0.0f, 1.0f);

	//farの設定
	m_fFar = length * 2.0f;

	return true;
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

