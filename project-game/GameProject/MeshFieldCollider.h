/*==============================================================================

    MeshFieldCollider.h - メッシュフィールドコライダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/31
==============================================================================*/
#ifndef _MESH_FIELD_COLLIDER_H_
#define _MESH_FIELD_COLLIDER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Collider.h"

/*------------------------------------------------------------------------------
	構造体定義
------------------------------------------------------------------------------*/
struct POLYGON
{
	int nVtxIndex[ 3];		//頂点のインデックス番号
	Vector3 Normal;			//面法線
};

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class MeshFieldCollider : public Collider
{
public:
	static Component* Create( GameObject* gameObject);

	MeshFieldCollider( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void OnCollision( Collider *pCollider);

	void SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight);
	void SetBlockSize( float Width, float Height);
	void SetSize( float Width, float Height);

	float GetHeight( const Vector3& Pos);
	
	virtual void Save( Text& text);
	virtual void Load( Text& text);

private:
	int m_nNumBlockX;			//横の分割数
	int m_nNumBlockZ;			//縦の分割数
	float m_fWidth;				//幅
	float m_fHeight;			//高さ
	float m_fBlockWidth;		//ポリゴン1枚あたりの幅
	float m_fBlockHeight;		//ポリゴン1枚あたりの高さ

	Vector3 *m_pVtxPos;			//頂点
	POLYGON *m_pPolygon;		//ポリゴン

	int GetIndexPolygon( const Vector3& Pos);
	Vector3 GetPolygonNormal( int nIndex);
	bool IsPointInPolygon( int nIndex, const Vector3& Pos);
	
};

#endif

