/*==============================================================================

   MeshField.h - メッシュフィールド
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/31
==============================================================================*/
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class MeshFieldRenderer;
class MeshFieldCollider;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class MeshField : public Component
{
public:
	MeshField( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( void);

	void SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight);
	void SetBlockSize( float Width, float Height);
	void SetSize( float Width, float Height);

private:
	MeshFieldRenderer *m_pMeshFieldRenderer;		//メッシュフィールド描画クラス
	MeshFieldCollider *m_pMeshFieldCollider;		//メッシュフィールドコライダー

	int m_nNumBlockX;			//横の分割数
	int m_nNumBlockZ;			//縦の分割数
	float m_fWidth;				//幅
	float m_fHeight;			//高さ
	float m_fBlockWidth;		//ポリゴン1枚あたりの幅
	float m_fBlockHeight;		//ポリゴン1枚あたりの高さ

	float *m_pVertexHeight;		//頂点ごとの高さ
};

#endif

