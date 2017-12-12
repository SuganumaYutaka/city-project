/*==============================================================================

    Building.cpp - 町の自動生成ー建物
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Building.h"
#include "Face.h"
#include "BuildingView.h"
#include "GameObject.h"
#include "ComponentInclude.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define BUILDING_HEIGHT (5.0f)

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Building::~Building()
{
	
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void Building::Init( BlockAttribute* attribute, GameObject* parent, const std::vector<Vector3> vertices)
{
	//ジオメトリの生成


	//Viewの生成
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	auto view = gameObject->AddComponent<BuildingView>();
	view->SetBuilding( this, attribute);
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void Building::Init( BlockAttribute* attribute, GameObject* parent, const Vector3& topLeft, const Vector3& topRight, const Vector3& bottomLeft, const Vector3& bottomRight)
{
	//頂点を格納
	std::vector<Vector3> vertices;
	vertices.resize( 4);
	vertices[0] = topLeft;
	vertices[1] = topRight;
	vertices[2] = bottomRight;
	vertices[3] = bottomLeft;

	//ジオメトリの生成


	//Viewの生成
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	auto view = gameObject->AddComponent<BuildingView>();
	view->SetBuilding( this, attribute);
}
