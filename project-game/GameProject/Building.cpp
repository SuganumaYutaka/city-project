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
void Building::Init( BlockAttribute* attribute, GameObject* parent, HalfEdgeDataStructure::Face* face, const std::vector<Vector3> vertices)
{
	m_Face = face;

	//頂点を格納
	m_Vertices = vertices;

	//TODO: 面積に応じてビルの高さを設定する
	m_Height = BUILDING_HEIGHT;

	//Viewの生成
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	auto view = gameObject->AddComponent<BuildingView>();
	view->SetBuilding( this, attribute);
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void Building::Init( BlockAttribute* attribute, GameObject* parent, HalfEdgeDataStructure::Face* face, const Vector3& topLeft, const Vector3& topRight, const Vector3& bottomLeft, const Vector3& bottomRight)
{
	m_Face = face;

	//頂点を格納
	m_Vertices.resize( 4);
	m_Vertices[0] = topLeft;
	m_Vertices[1] = topRight;
	m_Vertices[2] = bottomRight;
	m_Vertices[3] = bottomLeft;

	//TODO: 面積に応じてビルの高さを設定する
	m_Height = BUILDING_HEIGHT;

	//Viewの生成
	auto gameObject = new GameObject( parent);
	gameObject->IsCreatedByOtherComponent = true;
	auto view = gameObject->AddComponent<BuildingView>();
	view->SetBuilding( this, attribute);
}
