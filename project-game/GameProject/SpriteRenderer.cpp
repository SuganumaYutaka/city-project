/*==============================================================================

    SpriteRenderer.cpp - 2Dポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* SpriteRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<SpriteRenderer>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
SpriteRenderer::SpriteRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerUI;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_2D) * 4,				//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,					//頂点バッファの使用方法(速さに影響)
		0,									//FVF(頂点フォーマット)
		D3DPOOL_MANAGED,					//メモリの管理(MANAGEDはデバイスにおまかせ)
		&m_pVtxBuff,						//頂点バッファ管理インターフェイス
		NULL)))
	{
		//エラー
		assert( false);
		return;
	}

	//位置の設定
	//m_pTransform->SetLocalPosition( Vector3( SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	//大きさの設定
	//m_pTransform->SetLocalScale( Vector3( 100.0f, 100.0f, 1.0f));

	//回転量の設定
	m_fRotation = 0.0f;

	//色の設定
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//頂点設定
	SetVtxBuffer();

	//マテリアル
	m_pMaterial = new Material();

	//シェーダー
	m_pMaterial->SetShader( eShaderSprite);
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void SpriteRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//頂点バッファの解放
	if( m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//マテリアルの解放
	if (m_pMaterial != NULL)
	{
		delete m_pMaterial;
		m_pMaterial = NULL;
	}
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void SpriteRenderer::Update( void)
{
	//頂点設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void SpriteRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_2D));

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0,	2);

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void SpriteRenderer::SetVtxBuffer( void)
{
	//半径・角度を算出
	Vector3 pos = m_pTransform->GetLocalPosition();
	Vector3 scale = m_pTransform->GetLocalScale();
	float fRadius = sqrt( ( scale.x * 0.5f) * (  scale.x * 0.5f) + (  scale.y * 0.5f) * (  scale.y * 0.5f));
	float fAngle = atan2( scale.y,  scale.x);

	//ロック
	VERTEX_2D* pVtx;		//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	//頂点設定
	//座標（Z字）
	pVtx[ 0].Pos = D3DXVECTOR3( pos.x + fRadius * cosf( float( m_fRotation +fAngle + D3DX_PI)),	pos.y + fRadius * sinf( float( m_fRotation +fAngle + D3DX_PI)),	0.0f);
	pVtx[ 1].Pos = D3DXVECTOR3( pos.x + fRadius * cosf( float( m_fRotation -fAngle)),			pos.y + fRadius * sinf( float( m_fRotation -fAngle)),			0.0f);
	pVtx[ 2].Pos = D3DXVECTOR3( pos.x + fRadius * cosf( float( m_fRotation -fAngle + D3DX_PI)),	pos.y + fRadius * sinf( float( m_fRotation -fAngle + D3DX_PI)),	0.0f);
	pVtx[ 3].Pos = D3DXVECTOR3( pos.x + fRadius * cosf( float( m_fRotation +fAngle)),			pos.y + fRadius * sinf( float( m_fRotation +fAngle)),			0.0f);
	
	//頂点カラー
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV座標（Z字）
	pVtx[ 0].Tex = D3DXVECTOR2( m_TextureUV.UV[ 0].x, m_TextureUV.UV[ 0].y);
	pVtx[ 1].Tex = D3DXVECTOR2( m_TextureUV.UV[ 1].x, m_TextureUV.UV[ 1].y);
	pVtx[ 2].Tex = D3DXVECTOR2( m_TextureUV.UV[ 2].x, m_TextureUV.UV[ 2].y);
	pVtx[ 3].Tex = D3DXVECTOR2( m_TextureUV.UV[ 3].x, m_TextureUV.UV[ 3].y);

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void SpriteRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void SpriteRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void SpriteRenderer::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "Pass")
		{
			text.ForwardPositionToNextWord();
			m_nPass = std::stoi( text.GetWord());
		}

		else if (text.GetWord() == "Material")
		{
			text.ForwardPositionToNextWord();
			m_pMaterial->Load(text);
		}
		else if (text.GetWord() == "Color")
		{
			text.ForwardPositionToNextWord();
		
			m_Color.r = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Color.g = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Color.b = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Color.a = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "TextureUV")
		{
			text.ForwardPositionToNextWord();
		
			m_TextureUV.ConvertFromString( text.GetAllText(), text.GetPosition());
		}
		else if (text.GetWord() == "Rotation")
		{
			text.ForwardPositionToNextWord();
		
			m_fRotation = std::stof( text.GetWord());
		}

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
void SpriteRenderer::Save(Text& text)
{
	StartSave(text);

	text += "Pass " + std::to_string(m_nPass) + '\n';
	m_pMaterial->Save(text);
	text += "Color " 
		+ std::to_string(m_Color.r) + ' '
		+ std::to_string(m_Color.g) + ' '
		+ std::to_string(m_Color.b) + ' '
		+ std::to_string(m_Color.a) + '\n';
	text += "TextureUV " + m_TextureUV.ConvertToString() + '\n';
	text += "Rotation " + std::to_string( m_fRotation) + '\n';

	EndSave( text);
}
