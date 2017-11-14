/*==============================================================================

    ParticleRenderer.h - パーティクル描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/16
==============================================================================*/
#ifndef _PARTICLE_RENDERER_H_
#define _PARTICLE_RENDERER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"
#include "ShaderManager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Material;

/*------------------------------------------------------------------------------
	構造体定義
------------------------------------------------------------------------------*/
//頂点データ
typedef struct
{
	D3DXVECTOR3 Pos;		//位置
	D3DCOLOR Color;			//頂点カラー
	float Size;				//サイズ
	D3DXVECTOR3 Dir;		//初速度と方向
	float Life;				//継続時間（終了後ループ）
	float Start;			//開始時間（Lifeからどれくらい手前か）
}VERTEX_PARTICLE;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class ParticleRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	ParticleRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);
	void LoadTexture( std::string FileName);
	
	void SetShader( EShaderType Type);

	void SetColor( float R, float G, float B, float A) { m_Color = D3DXCOLOR( R, G, B, A); m_bNeedUpdateVtx = true;}
	void SetAlpha( float Center, float Range) { m_Color.a = Center; m_AlphaRange = Range; m_bNeedUpdateVtx = true;}
	void SetPosRange( Vector3 Range) { m_PosRange = Range; m_bNeedUpdateVtx = true;}
	void SetSize( float Center, float Range) { m_SizeCenter = Center; m_SizeRange = Range; m_bNeedUpdateVtx = true;}
	void SetDirection( Vector3 Center, Vector3 Range) { m_DirCenter = Center; m_DirRange = Range; m_bNeedUpdateVtx = true;}
	void SetSpeed( float Center, float Range) { m_SpeedCenter = Center; m_SpeedRange = Range; m_bNeedUpdateVtx = true;}
	void SetLife( float Center, float Range) { m_LifeCenter = Center; m_LifeRenge = Range; m_bNeedUpdateVtx = true;}
	void SetStartRange( float Range) { m_StartRange = Range; m_bNeedUpdateVtx = true;}

	void SetNumParticle( int NumParticle);

	virtual void Save( Text& text);
	virtual void Load( Text& text);
	
private:
	void SetVtxBuffer( void);

	Material*	m_pMaterial;			//マテリアル
	D3DXCOLOR	m_Color;			//色（0.0f～1.0f）
	float		m_AlphaRange;		//アルファ値（範囲）
	Vector3		m_PosRange;			//位置（範囲）
	float		m_SizeCenter;		//サイズ（中央値）
	float		m_SizeRange;		//サイズ（範囲）
	Vector3		m_DirCenter;		//方向（中央値）
	Vector3		m_DirRange;			//方向（範囲）
	float		m_SpeedCenter;		//初速度（中央値）
	float		m_SpeedRange;		//初速度（範囲）
	float		m_LifeCenter;		//継続時間（中央値）
	float		m_LifeRenge;		//継続時間（範囲）
	float		m_StartRange;		//開始時間（範囲）

	int m_nNumParticle;				//パーティクルの数
	bool m_bNeedUpdateVtx;			//頂点バッファの更新が必要か
};

#endif

