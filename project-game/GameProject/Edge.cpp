/*==============================================================================

    Edge.cpp - ハーフエッジ構造ー辺
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Edge::Edge( HalfEdgeDataStructure::Model* model, Vertex* start, Vertex* end)
	: m_Model(model), m_Start(start), m_End(end), m_Left(NULL), m_Right(NULL), m_Attribute(NULL)
{
	model->RegisterEdge(this); 
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
Edge::~Edge()
{ 
	//ハーフエッジの削除
	if (m_Right)
	{
		delete m_Right;
	}
	if (m_Left)
	{
		delete m_Left;
	}

	m_Model->UnregisterEdge( this);
	if( m_Attribute)
	{
		m_Attribute->OnDeleteEdge();
	}
}

/*------------------------------------------------------------------------------
	ベクトルの取得
------------------------------------------------------------------------------*/
Vector3 Edge::GetVector(void)
{
	return m_End->GetPosition() - m_Start->GetPosition();
}

/*------------------------------------------------------------------------------
	分割する
------------------------------------------------------------------------------*/
bool Edge::Divide(float rateFromStart, Vertex** ppOut)
{
	//引数チェック
	if (rateFromStart < 0.0f || rateFromStart > 1.0f)
	{
		return false;
	}

	//頂点の生成
	Vector3 direction = m_End->GetPosition() - m_Start->GetPosition();
	Vector3 position = m_Start->GetPosition() + direction * rateFromStart;
	auto vertex = m_Model->CreateVertex( position);
	
	//頂点を出力
	if ( ppOut)
	{
		*ppOut = vertex;
	}

	//辺の生成
	auto edge = m_Model->CreateEdge( vertex, m_End);

	//辺に点を登録
	this->SetEnd( vertex);

	//右側面の設定
	if (this->GetRight())
	{
		auto rightnext = new HalfEdge( m_Model, edge->GetEnd());
		edge->SetRight( rightnext);
		rightnext->SetFace( this->GetRight()->GetFace());
		rightnext->SetEdge( edge);
		rightnext->SetNext( this->GetRight()->GetNext());
		this->GetRight()->SetNext( rightnext);
		this->GetRight()->SetEnd( vertex);
	}
	//左側面の設定
	if (this->GetLeft())
	{
		auto leftnext = new HalfEdge( m_Model, this->GetStart());
		edge->SetLeft( this->GetLeft());
		leftnext->SetFace( this->GetLeft()->GetFace());
		this->GetLeft()->SetEdge( edge);
		this->SetLeft( leftnext);
		leftnext->SetEdge( this);
		leftnext->SetNext( edge->GetLeft()->GetNext());
		edge->GetLeft()->SetNext( leftnext);
		edge->GetLeft()->SetEnd( vertex);
	}
	
	//ハーフエッジのペアを設定
	if( this->GetRight() && this->GetLeft())
	{
		this->GetLeft()->SetPair( this->GetRight());
		this->GetRight()->SetPair( this->GetLeft());
		edge->GetLeft()->SetPair( edge->GetRight());
		edge->GetRight()->SetPair( edge->GetLeft());
	}

	//点に辺を登録
	vertex->RegisterEdge( this);
	vertex->RegisterEdge( edge);
	edge->GetEnd()->UnregisterEdge( this);
	edge->GetEnd()->RegisterEdge( edge);

	return true;
}

/*------------------------------------------------------------------------------
	移動による更新
------------------------------------------------------------------------------*/
void Edge::UpdateByMove(void)
{
	if( m_Attribute)
	{
		m_Attribute->Update();
	}

	//隣接する面に知らせる
	//右
	if (m_Right)
	{
		m_Right->GetFace()->UpdateByMove();
	}

	//左
	if (m_Left)
	{
		m_Left->GetFace()->UpdateByMove();
	}
}
