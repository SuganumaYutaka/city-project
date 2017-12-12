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
	void Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, BuildingRule* rule);
	void Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& center, float radius, BuildingRule* rule);

	bool UpdateView( const Vector3& bottomLeftPosition, const Vector3& normal);
	bool UpdateView( const Vector3& bottomLeftPosition, const Vector3& center, float radius);
	
	void AddFloor( Floor* floor);
	void SubFloor( Floor* floor);

	float GetHeight(void) { return m_Height;}
	float GetWidth( void) { return m_Width;}
	const Vector3& GetBottomLeftPosition( void) { return m_BottomLeftPosition;}

	void LoadTexture( std::string fileName);

private:
	float m_Height;
	float m_Width;
	Vector3 m_BottomLeftPosition;
	std::list<Floor*> m_Floors;
	WallRenderer* m_Renderer;

	int CulcCountVertex( void);
	int CulcCountVertexCurve( float radius);

	int CulcCountPolygon( void);
	int CulcCountPolygonCurve( float radius);
};

#endif

