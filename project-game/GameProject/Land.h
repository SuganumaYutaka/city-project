/*==============================================================================

    Land.h - 町の自動生成ー土地
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
class GameObject;
class MeshPolygonRenderer;
class TrafficLand;
class RoadAttribute;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Land
{
public:
	Land( GameObject* parent);
	~Land();
	void Init( const std::vector<Vector3>& vertices);
	void OnDeleteBlock( void);

	const std::vector<Vector3>& GetVertices( void) { return m_Vertices;}
	Vector3 CulcCenterPosition( void);

	void SetTraffic( std::list<RoadAttribute*> roadAttributes);

private:
	std::vector<Vector3> m_Vertices;

	GameObject* m_GameObject;
	MeshPolygonRenderer* m_Renderer;
	TrafficLand* m_TrafficLand;
	
};

#endif

