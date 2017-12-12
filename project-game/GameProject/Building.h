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
class BlockAttribute;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Building
{
private:
	HalfEdgeDataStructure::Face* m_Face;

public:
	Building() : m_Face( NULL){}
	~Building();

	void Init( BlockAttribute* attribute, GameObject* parent, const std::vector<Vector3> vertices);
	void Init( BlockAttribute* attribute, GameObject* parent, const Vector3& topLeft, const Vector3& topRight, const Vector3& bottomLeft, const Vector3& bottomRight);

	HalfEdgeDataStructure::Face* GetFace( void){ return m_Face;}
};

#endif

