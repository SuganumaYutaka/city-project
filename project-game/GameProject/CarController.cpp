/*==============================================================================
	
	CarController.h - 交通システムー車
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "CarController.h"
#include "GameObject.h"
#include "ComponentInclude.h"

#include "TrafficRoad.h"
#include "TrafficJunction.h"
#include "TrafficBuilding.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* CarController::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<CarController>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
CarController::CarController( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_CurrentRoad = NULL;
	m_NextJunction = NULL;
	m_TargetBuilding = NULL;

	//ステートの設定
	m_States[ eCarStateNeutral] = new CarStateNeutral();
	m_CurrentState = m_States[ eCarStateNeutral];

	//描画の設定
	auto renderer = m_pGameObject->AddComponent<XModelRenderer>();
	renderer->LoadXModel( "data/MODEL/car.x");
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void CarController::Init(TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficBuilding* targetBuilding)
{
	m_CurrentRoad = currentRoad;
	m_NextJunction = nextJunction;
	m_TargetBuilding = targetBuilding;
}


/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void CarController::Uninit( void)
{
	//ステートの解放
	for (auto state : m_States)
	{
		delete state.second;
	}
}

/*------------------------------------------------------------------------------
	更新処理
------------------------------------------------------------------------------*/
void CarController::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	道路の上を車が進んだ距離を算出する
------------------------------------------------------------------------------*/
float CarController::CulcMoveDistanceOnRoad(void)
{
	//「正規化したベクトルNと任意ベクトルAとの内積は、AをNに投影した時の長さ（符号付き）となる」性質を利用する
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = m_CurrentRoad->GetVector( m_NextJunction);
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	float length = Vector3::Dot( roadVector.Normalize(), vector);

	return length;
}

/*------------------------------------------------------------------------------
	道路の上を車が進んだ距離の割合を算出する
------------------------------------------------------------------------------*/
float CarController::CulcMoveRateOnRoad(void)
{
	//「正規化したベクトルNと任意ベクトルAとの内積は、AをNに投影した時の長さ（符号付き）となる」性質を利用する
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = m_CurrentRoad->GetVector( m_NextJunction);
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	float length = Vector3::Dot( roadVector.Normalize(), vector);

	//割合にするために長さで割る
	float rate = length / roadVector.Length();

	return rate;
}

/*------------------------------------------------------------------------------
	車が次の交差点までに残っている距離を算出する
------------------------------------------------------------------------------*/
float CarController::CulcRemainDistanceOnRoad(void)
{
	//「正規化したベクトルNと任意ベクトルAとの内積は、AをNに投影した時の長さ（符号付き）となる」性質を利用する
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = m_CurrentRoad->GetVector( m_NextJunction);
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	float length = Vector3::Dot( roadVector.Normalize(), vector);

	return roadVector.Length() - length;
}

/*------------------------------------------------------------------------------
	右折
------------------------------------------------------------------------------*/
bool CarController::TurnRight(void)
{
	//変更先の道路を取得
	auto nextRoad = m_NextJunction->GetRightRoad( m_CurrentRoad);
	if (!nextRoad)
	{
		return false;
	}

	//交差点を変更
	m_NextJunction = nextRoad->GetOtherJunction( m_NextJunction);

	//道路を変更
	m_CurrentRoad->UnregisterCar( this);
	m_CurrentRoad = nextRoad;
	nextRoad->RegisterCar( this);

	return true;
}

/*------------------------------------------------------------------------------
	左折
------------------------------------------------------------------------------*/
bool CarController::TurnLeft(void)
{
	//変更先の道路を取得
	auto nextRoad = m_NextJunction->GetLeftRoad( m_CurrentRoad);
	if (!nextRoad)
	{
		return false;
	}

	//交差点を変更
	m_NextJunction = nextRoad->GetOtherJunction( m_NextJunction);

	//道路を変更
	m_CurrentRoad->UnregisterCar( this);
	m_CurrentRoad = nextRoad;
	nextRoad->RegisterCar( this);

	return true;
}

/*------------------------------------------------------------------------------
	直進
------------------------------------------------------------------------------*/
bool CarController::GoStraight(void)
{
	//変更先の道路を取得
	auto nextRoad = m_NextJunction->GetStraightRoad( m_CurrentRoad);
	if (!nextRoad)
	{
		return false;
	}

	//交差点を変更
	m_NextJunction = nextRoad->GetOtherJunction( m_NextJunction);

	//道路を変更
	m_CurrentRoad->UnregisterCar( this);
	m_CurrentRoad = nextRoad;
	nextRoad->RegisterCar( this);

	return true;
}
