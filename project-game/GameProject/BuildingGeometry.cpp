/*==============================================================================

    BuildingGeometry.cpp - 建物の自動生成ー建物ジオメトリ
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "BuildingGeometry.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Land.h"
#include "Shape.h"

#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* BuildingGeometry::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BuildingGeometry>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
BuildingGeometry::BuildingGeometry( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void BuildingGeometry::Uninit( void)
{
	if (m_Rule)
	{
		delete m_Rule;
	}

	if (m_Land)
	{
		delete m_Land;
	}
	
	for (Shape* shape : m_Shapes)
	{
		delete shape;
	}
	m_Shapes.clear();
}

/*------------------------------------------------------------------------------
	初期化処理
------------------------------------------------------------------------------*/
bool BuildingGeometry::Init( const std::vector<Vector3>& vertices, BuildingRule* rule)
{
	//土地の生成
	m_Land = new Land( m_pGameObject);
	
	//Position(World座標)を土地の中心に
	Vector3 vec02 = vertices[2] - vertices[0];
	auto center = vertices[0] + vec02 * 0.5f;
	m_pTransform->SetWorldPosition( center);

	//土地が道路と平行となるように回転
	Vector3 vec21 = vertices[1] - vertices[2];
	m_pTransform->SetWorldRotationLookDirection( vec21);

	//相対位置を算出して土地を設定
	std::vector<Vector3> landVertices = vertices;
	for (auto& vertex : landVertices)
	{
		vertex -= center;
	}
	m_Land->Init( landVertices);

	//形状の生成
	rule->ProceduralShape( this);

	m_Rule = rule;

	return true;
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void BuildingGeometry::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	ジオメトリを確定させる
------------------------------------------------------------------------------*/
void BuildingGeometry::ConfirmGeometry(void)
{
	if (m_Shapes.size() == 1)
	{
		//壁の融合
		m_Shapes.front()->ConfirmShape();
	}
	else
	{
		//TODO:Shapeの融合
	}
}
