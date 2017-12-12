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
class BuildingRuleFactory;

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
	BlockView* m_View;
	BuildingRuleFactory* m_BuildingRuleFactory;
	
public:
	BlockAttribute( GameObject* parent, BuildingRuleFactory* buildingRuleFactory);
	~BlockAttribute();
	void Init( void) override;
	void Update( void) override;
	void UnregisterView( void){ m_View = NULL;}

	BuildingRuleFactory* GetBuildingRuleFactory( void) { return m_BuildingRuleFactory;}
};

//ファクトリー
class CityAttributeFactory : public HalfEdgeDataStructure::AttributeFactory
{
private:
	GameObject* m_Parent;
	BuildingRuleFactory* m_BuildingRuleFactory;

public:
	CityAttributeFactory( GameObject* parent, BuildingRuleFactory* buildingRuleFactory) : m_Parent( parent), m_BuildingRuleFactory(buildingRuleFactory) {}

	virtual HalfEdgeDataStructure::VertexAttribute* CreateVertexAttribute( void) { return new JunctionAttribute( m_Parent); }
	virtual HalfEdgeDataStructure::EdgeAttribute* CreateEdgeAttribute( void) { return new RoadAttribute( m_Parent); }
	virtual HalfEdgeDataStructure::FaceAttribute* CreateFaceAttribute( void){ return new BlockAttribute( m_Parent, m_BuildingRuleFactory); }
};

#endif

