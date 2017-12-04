/*==============================================================================

    CityAttribute.h - 町の自動生成ー属性
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/29
==============================================================================*/
#ifndef _CITY_ATTRIBUTE_H_
#define _CITY_ATTRIBUTE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Attribute.h"
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class JunctionView;
class RoadView;
class BlockView;
class GameObject;
class Building;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
//点（交差点）
class JunctionAttribute : public HalfEdgeDataStructure::VertexAttribute
{
private:
	JunctionView* m_View;

public:
	JunctionAttribute( GameObject* parent);
	~JunctionAttribute();
	void Init( void) override;
	void Update( void) override;
	void UnregisterView( void){ m_View = NULL;}

	const Vector3& GetPosition(void);
};

//辺（道路）
class RoadAttribute : public HalfEdgeDataStructure::EdgeAttribute
{
private:
	RoadView* m_View;
	float Width;

public:
	RoadAttribute( GameObject* parent);
	~RoadAttribute();
	void Init( void) override;
	void Update( void) override;
	void UnregisterView( void){ m_View = NULL;}

	float GetWidth( void) const{ return Width;}
	void SetWidth( float width){ Width = width; Update();}

	float GetLength( void);
	Vector3 GetCenterPosition( void);
	Vector3 GetVector( void);
};

//面（区画）
class BlockAttribute : public HalfEdgeDataStructure::FaceAttribute
{
private:
	//建物の生成用構造体定義
	//辺（連続する頂点）
	typedef struct
	{
		std::vector<Vector3> vertices;
		Vector3 vector;
	}FaceEdge;

	//土地（連続する頂点）
	typedef struct
	{
		std::vector<Vector3> vertices;
		bool canCreateBuilding;
	}FaceLand;

	BlockView* m_View;
	std::list<Building*> m_Buildings;
	GameObject* m_ViewGameObject;

	bool NarrowLand( Vector3& start, Vector3& end, float value);
	bool MoveLand( Vector3& start, Vector3& end, float value);

public:
	BlockAttribute( GameObject* parent);
	~BlockAttribute();
	void Init( void) override;
	void Update( void) override;
	void UnregisterView( void){ m_View = NULL;}

	const std::list<Building*>& GetBuildings( void) { return m_Buildings;}

	bool CreateBuilding( void);
};

//ファクトリー
class CityAttributeFactory : public HalfEdgeDataStructure::AttributeFactory
{
private:
	GameObject* m_Parent;

public:
	CityAttributeFactory( GameObject* parent) : m_Parent( parent) {}

	virtual HalfEdgeDataStructure::VertexAttribute* CreateVertexAttribute( void) { return new JunctionAttribute( m_Parent); }
	virtual HalfEdgeDataStructure::EdgeAttribute* CreateEdgeAttribute( void) { return new RoadAttribute( m_Parent); }
	virtual HalfEdgeDataStructure::FaceAttribute* CreateFaceAttribute( void){ return new BlockAttribute( m_Parent); }
};

#endif

