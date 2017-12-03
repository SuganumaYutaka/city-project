/*==============================================================================

    Building.h - 町の自動生成ー建物
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/
#ifndef _BUILDING_H_
#define _BUILDING_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Vector3.h"
#include <vector>

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Face;
}
class BuildingView;
class GameObject;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Building
{
private:
	std::vector<Vector3> m_Vertices;
	float m_Height;
	HalfEdgeDataStructure::Face* m_Face;
	BuildingView* m_View;

public:
	Building( GameObject* parent, HalfEdgeDataStructure::Face* face, const std::vector<Vector3> vertices);
	Building( GameObject* parent, HalfEdgeDataStructure::Face* face, const Vector3& topLeft, const Vector3& topRight, const Vector3& bottomLeft, const Vector3& bottomRight);
	~Building();

	void Init( void);

	const std::vector<Vector3>& GetVertices( void) { return m_Vertices;}
	float GetHeight( void) { return m_Height;}
	HalfEdgeDataStructure::Face* GetFace( void){ return m_Face;}

	void UnregisterView( void){ m_View = NULL;}
};

#endif

