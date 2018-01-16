/*==============================================================================

    Wall.h - 建物の自動生成ー壁
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _WALL_H_
#define _WALL_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Floor;
class WallRenderer;
class GameObject;
class BuildingRule;
class Texture;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Wall
{
public:
	Wall( GameObject* buildingObject);
	~Wall();
	void InitDefault( D3DXMATRIX shapeMatrix, float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, BuildingRule* rule);
	void InitCurve( D3DXMATRIX shapeMatrix, float height, float width, const Vector3& bottomLeftPosition, BuildingRule* rule);

	bool UpdateView( D3DXMATRIX shapeMatrix);
	
	void AddFloor( Floor* floor);
	void SubFloor( Floor* floor);

	float GetHeight(void) { return m_Height;}
	float GetWidth( void) { return m_Width;}
	const Vector3& GetBottomLeftPosition( void) { return m_BottomLeftPosition;}
	const Vector3& GetNormal( void) { return m_Normal;}

	void LoadTexture( std::string fileName);
	void FusionSameShape( Wall* other);
	bool ChangeRingList( void);
	bool Collision( Wall* other);
	Vector3 GetVector( void);

private:
	float m_Height;
	float m_Width;
	Vector3 m_BottomLeftPosition;
	Vector3 m_Normal;
	std::list<Floor*> m_Floors;
	WallRenderer* m_Renderer;

	int CulcCountVertex( void);
	int CulcCountPolygon( void);
};

#endif

