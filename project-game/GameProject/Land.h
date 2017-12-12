/*==============================================================================

    Land.h - 建物の自動生成ー土地
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _LAND_H_
#define _LAND_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class MeshPolygonRenderer;
class GameObject;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Land
{
public:
	Land( GameObject* buildingObject);
	void Init( const std::vector<Vector3>& vertices);

	const std::vector<Vector3>& GetVertices( void) { return m_Vertices;}

	Vector3 CulcCenterPosition( void);

	//TODO:面積を求める

private:
	std::vector<Vector3> m_Vertices;
	MeshPolygonRenderer* m_Renderer;

};

#endif

