/*==============================================================================

    CityAttribute.h - ���̎��������[����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/29
==============================================================================*/
#ifndef _CITY_ATTRIBUTE_H_
#define _CITY_ATTRIBUTE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Attribute.h"
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class JunctionView;
class RoadView;
class BlockView;
class GameObject;
class BuildingRuleFactory;
class BuildingManager;
class CarManager;
class TrafficJunction;
class TrafficRoad;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define DEFAULT_ROAD_WIDTH (16.0f)			//�f�t�H���g�̓��H��

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
//�_�i�����_�j
class JunctionAttribute : public HalfEdgeDataStructure::VertexAttribute
{
private:
	JunctionView* m_View;
	TrafficJunction* m_TrafficJunction;

public:
	JunctionAttribute( GameObject* parent);
	~JunctionAttribute();
	void Init( void) override;
	void Update( void) override;
	void UnregisterView( void){ m_View = NULL;}

	const Vector3& GetPosition(void);
	TrafficJunction* GetTrafficJunction( void){ return m_TrafficJunction;}
};

//�Ӂi���H�j
class RoadAttribute : public HalfEdgeDataStructure::EdgeAttribute
{
private:
	RoadView* m_View;
	TrafficRoad* m_TrafficRoad;
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
	std::vector<Vector3> GetVertices( void);
	TrafficRoad* GetTrafficRoad( void){ return m_TrafficRoad;}
};

//�ʁi���j
class BlockAttribute : public HalfEdgeDataStructure::FaceAttribute
{
private:
	BlockView* m_View;
	BuildingRuleFactory* m_BuildingRuleFactory;
	BuildingManager* m_BuildingManager;
	CarManager* m_CarManager;

public:
	BlockAttribute( GameObject* parent, BuildingRuleFactory* buildingRuleFactory, BuildingManager* buildingManager, CarManager* carManager);
	~BlockAttribute();
	void Init( void) override;
	void Update( void) override;
	void UnregisterView( void){ m_View = NULL;}

	BuildingRuleFactory* GetBuildingRuleFactory( void) { return m_BuildingRuleFactory;}
	BuildingManager* GetBuildingManager( void) { return m_BuildingManager;}
	CarManager* GetCarManager( void){ return m_CarManager;}
};

//�t�@�N�g���[
class CityAttributeFactory : public HalfEdgeDataStructure::AttributeFactory
{
private:
	GameObject* m_Parent;
	BuildingRuleFactory* m_BuildingRuleFactory;
	BuildingManager* m_BuildingManager;
	CarManager* m_CarManager;

public:
	CityAttributeFactory( GameObject* parent, BuildingRuleFactory* buildingRuleFactory, BuildingManager* buildingManager, CarManager* carManager)
		: m_Parent( parent), m_BuildingRuleFactory(buildingRuleFactory), m_BuildingManager( buildingManager), m_CarManager( carManager) {}

	virtual HalfEdgeDataStructure::VertexAttribute* CreateVertexAttribute( void) { return new JunctionAttribute( m_Parent); }
	virtual HalfEdgeDataStructure::EdgeAttribute* CreateEdgeAttribute( void) { return new RoadAttribute( m_Parent); }
	virtual HalfEdgeDataStructure::FaceAttribute* CreateFaceAttribute( void){ return new BlockAttribute( m_Parent, m_BuildingRuleFactory, m_BuildingManager, m_CarManager); }
};

#endif

