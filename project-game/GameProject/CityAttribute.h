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
class Land;
class CityAttributeManager;

namespace HalfEdgeDataStructure
{
	class Model;
}

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
	CityAttributeManager* m_Manager;
	GameObject* m_GameObject;
	JunctionView* m_View;
	TrafficJunction* m_TrafficJunction;

public:
	JunctionAttribute( HalfEdgeDataStructure::Model* model, int id, CityAttributeManager* manager, GameObject* parent);
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
	CityAttributeManager* m_Manager;
	GameObject* m_GameObject;
	RoadView* m_View;
	TrafficRoad* m_TrafficRoad;
	float Width;

public:
	RoadAttribute( HalfEdgeDataStructure::Model* model, int id, CityAttributeManager* manager, GameObject* parent);
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
	CityAttributeManager* m_Manager;
	GameObject* m_GameObject;
	BlockView* m_View;
	std::vector<Land*> m_Lands;

public:
	BlockAttribute( HalfEdgeDataStructure::Model* model, int id, CityAttributeManager* manager, GameObject* parent);
	~BlockAttribute();
	void Init( void) override;
	void Update( void) override;
	void UnregisterView( void){ m_View = NULL;}

	int LinkLand( Land* land);
	void UnlinkLand( Land* land);
	void UnlinkLand( int landID);
};

#endif

