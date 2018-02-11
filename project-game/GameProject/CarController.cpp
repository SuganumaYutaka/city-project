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
#include "TrafficLand.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define FRICTION_SPEED_RATE (0.15f)
#define BRAKE_SPEED_RATE (0.5f)
#define ON_ROAD_PERMIT_RATE (0.1f)

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
CarController::CarController( GameObject* pGameObject) : m_Speed( 0.0f)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_CurrentRoad = NULL;
	m_NextJunction = NULL;
	m_TargetLand = NULL;

	//ステートの設定
	m_States[ eCarStateNeutral] = new CarStateNeutral( this);
	m_States[ eCarStateMoveOnRoad] = new CarStateMoveOnRoad( this);
	m_States[ eCarStateMoveTowardRoad] = new CarStateMoveTowardRoad( this);
	m_States[ eCarStateNearNextJunction] = new CarStateNearNextJunction( this);
	m_CurrentState = m_States[ eCarStateMoveTowardRoad];
	//m_CurrentState = m_States[ eCarStateNeutral];

	//描画の設定
	auto renderer = m_pGameObject->AddComponent<XModelRenderer>();
	renderer->LoadXModel( "data/MODEL/car.x");
}

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void CarController::Init(TrafficRoad* currentRoad, TrafficJunction* nextJunction, TrafficLand* targetLand)
{
	m_CurrentRoad = currentRoad;
	m_NextJunction = nextJunction;
	m_TargetLand = targetLand;
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
	//ステートの更新
	m_CurrentState->Update();

	//正面方向に移動
	m_pTransform->Move( m_pTransform->GetForward() * m_Speed);
	
	//摩擦による減速
	m_Speed -= ( m_Speed * FRICTION_SPEED_RATE);
}

/*------------------------------------------------------------------------------
	道路のベクトルを取得
------------------------------------------------------------------------------*/
Vector3 CarController::GetCurrentRoadVector(void)
{
	return m_CurrentRoad->GetVector( m_NextJunction);
}

/*------------------------------------------------------------------------------
	前方の車を取得
------------------------------------------------------------------------------*/
CarController* CarController::GetFrontCar(void)
{
	return m_CurrentRoad->GetFrontCar( this);
}

/*------------------------------------------------------------------------------
	車が道路上を走行しているか
------------------------------------------------------------------------------*/
bool CarController::CheckOnRoad(void)
{
	//正しい車線側にあるか
	if (!IsCorrectSideRoad())
	{
		return false;
	}

	//車線の中央を走行しているか
	float length = CulcLengthFromRoadCenter();
	float roadWidthHalf = m_CurrentRoad->GetWidth() * 0.5f;
	float permitWidth = roadWidthHalf * ON_ROAD_PERMIT_RATE;
	if (length > roadWidthHalf + permitWidth || length < roadWidthHalf - permitWidth)
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	車が正しい車線側にあるか
------------------------------------------------------------------------------*/
bool CarController::IsCorrectSideRoad( void)
{
	//外積を用いて判定
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = GetCurrentRoadVector();
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	if (Vector3::Cross(vector, roadVector).y >= 0.0f)
	{
		return true;
	}
	
	return false;
}

/*------------------------------------------------------------------------------
	道路の中央線からの距離を算出
------------------------------------------------------------------------------*/
float CarController::CulcLengthFromRoadCenter(void)
{
	//点と線分の距離を求める
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = GetCurrentRoadVector();
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;

	auto heightVector = roadVector * Vector3::Dot( roadVector.Normalize(), vector) / roadVector.Length() - vector;

	return heightVector.Length();
}

/*------------------------------------------------------------------------------
	道路の上を車が進んだ距離を算出する
------------------------------------------------------------------------------*/
float CarController::CulcMoveDistanceOnRoad(void)
{
	//「正規化したベクトルNと任意ベクトルAとの内積は、AをNに投影した時の長さ（符号付き）となる」性質を利用する
	auto otherJunctionPosition = m_CurrentRoad->GetOtherJunction( m_NextJunction)->GetPosition();
	auto roadVector = GetCurrentRoadVector();
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
	auto roadVector = GetCurrentRoadVector();
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
	auto roadVector = GetCurrentRoadVector();
	Vector3 vector = m_pTransform->GetWorldPosition() - otherJunctionPosition;
	float length = Vector3::Dot( roadVector.Normalize(), vector);

	return roadVector.Length() - length;
}

/*------------------------------------------------------------------------------
	右にある道路を取得
------------------------------------------------------------------------------*/
TrafficRoad* CarController::GetRightRoad(void)
{
	return m_NextJunction->GetRightRoad( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	左にある道路を取得
------------------------------------------------------------------------------*/
TrafficRoad* CarController::GetLeftRoad(void)
{
	return m_NextJunction->GetLeftRoad( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	直進する道路を取得
------------------------------------------------------------------------------*/
TrafficRoad* CarController::GetStraightRoad(void)
{
	return m_NextJunction->GetStraightRoad( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	右折できるか
------------------------------------------------------------------------------*/
bool CarController::CanTurnRight(void)
{
	return m_NextJunction->CanTurnRight( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	左折できるか
------------------------------------------------------------------------------*/
bool CarController::CanTurnLeft(void)
{
	return m_NextJunction->CanTurnLeft( m_CurrentRoad);
}

/*------------------------------------------------------------------------------
	直進できるか
------------------------------------------------------------------------------*/
bool CarController::CanGoStraight(void)
{
	return m_NextJunction->CanGoStraight( m_CurrentRoad);
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

/*------------------------------------------------------------------------------
	状態遷移
------------------------------------------------------------------------------*/
void CarController::ChangeState(ECarState next)
{
	m_CurrentState = m_States[ next];
	m_CurrentState->Init();
}

/*------------------------------------------------------------------------------
	ブレーキを踏む
------------------------------------------------------------------------------*/
void CarController::BrakeSpeed(void)
{
	m_Speed -= ( m_Speed * BRAKE_SPEED_RATE);
}

/*------------------------------------------------------------------------------
	走行中の道路消去時の処理
------------------------------------------------------------------------------*/
void CarController::OnDeleteCurrentRoad(void)
{
	m_CurrentRoad = NULL;
	SetActive( false);
}

/*------------------------------------------------------------------------------
	道路から外れる
------------------------------------------------------------------------------*/
void CarController::OutRoad(void)
{
	if( m_CurrentRoad)
	{
		m_CurrentRoad->UnregisterCar( this);
		m_CurrentRoad = NULL;
	}
	SetActive( false);
}
