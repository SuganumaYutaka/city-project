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
class Building;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
//�_�i�����_�j
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

//�Ӂi���H�j
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

//�ʁi���j
class BlockAttribute : public HalfEdgeDataStructure::FaceAttribute
{
private:
	BlockView* m_View;
	std::vector<Building*> m_Buildings;
	GameObject* m_ViewGameObject;

	bool CreateBuilding( void);

public:
	BlockAttribute( GameObject* parent);
	~BlockAttribute();
	void Init( void) override;
	void Update( void) override;
	void UnregisterView( void){ m_View = NULL;}

};

//�t�@�N�g���[
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

