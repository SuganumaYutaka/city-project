/*==============================================================================
	
	TrafficRoad.cpp - 交通システムー交差点
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TrafficRoad.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "CityAttribute.h"

#include "Edge.h"
#include "Vertex.h"
#include "HalfEdge.h"

#include "TrafficJunction.h"
#include "CarController.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* TrafficRoad::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<TrafficRoad>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
TrafficRoad::TrafficRoad( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_Attribute = NULL;
	m_IsUpdatedAttribute = false;

	m_LeftSideCars.clear();
	m_RightSideCars.clear();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void TrafficRoad::Uninit( void)
{
	m_Attribute->UnregisterView();
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void TrafficRoad::Update( void)
{
	if (m_IsUpdatedAttribute)
	{

		m_IsUpdatedAttribute = false;
	}

	

}

/*------------------------------------------------------------------------------
	属性情報の設定
------------------------------------------------------------------------------*/
void TrafficRoad::SetAttribute( RoadAttribute* attribute)
{ 
	if( m_Attribute)
	{	
		return;
	}
	
	m_Attribute = attribute;
	m_IsUpdatedAttribute = true;
}

/*------------------------------------------------------------------------------
	車の登録
------------------------------------------------------------------------------*/
void TrafficRoad::RegisterCar(CarController* car, TrafficJunction* nextJunction)
{
	if ( !m_Attribute || !nextJunction->GetAttribute())
	{
		return;
	}

	UnregisterCar( car);

	//次に到達する交差点の情報から左右のどちらを通行するか決定
	if (nextJunction->GetAttribute()->GetVertex() == m_Attribute->GetEdge()->GetEnd())
	{
		m_LeftSideCars.push_back( car);
	}
	else
	{
		m_RightSideCars.push_back( car);
	}
}
	
/*------------------------------------------------------------------------------
	車の登録を解除
------------------------------------------------------------------------------*/
void TrafficRoad::UnregisterCar(CarController* car)
{
	for (auto ite = m_LeftSideCars.begin(); ite != m_LeftSideCars.end(); ++ite)
	{
		if (*ite == car)
		{
			m_LeftSideCars.erase( ite);
			return;
		}
	}
	for (auto ite = m_RightSideCars.begin(); ite != m_RightSideCars.end(); ++ite)
	{
		if (*ite == car)
		{
			m_RightSideCars.erase( ite);
			return;
		}
	}
}

/*------------------------------------------------------------------------------
	車の取得
------------------------------------------------------------------------------*/
std::list<CarController*>* TrafficRoad::GetCars(TrafficJunction* nextJunction)
{
	if ( !m_Attribute || !nextJunction->GetAttribute())
	{
		return NULL;
	}

	//次に到達する交差点の情報から左右のどちらを通行するか決定
	if (nextJunction->GetAttribute()->GetVertex() == m_Attribute->GetEdge()->GetEnd())
	{
		return &m_LeftSideCars;
	}
	else
	{
		return &m_RightSideCars;
	}

	return NULL;
}
