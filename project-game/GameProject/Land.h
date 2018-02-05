/*==============================================================================

    Land.h - ���̎��������[�y�n
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _LAND_H_
#define _LAND_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class MeshPolygonRenderer;
class TrafficLand;
class RoadAttribute;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Land
{
public:
	Land( GameObject* parent);
	~Land();
	void Init( const std::vector<Vector3>& vertices);
	void OnDeleteBlock( void);

	const std::vector<Vector3>& GetVertices( void) { return m_Vertices;}
	Vector3 CulcCenterPosition( void);

	void SetTraffic( std::list<RoadAttribute*> roadAttributes);

private:
	std::vector<Vector3> m_Vertices;

	GameObject* m_GameObject;
	MeshPolygonRenderer* m_Renderer;
	TrafficLand* m_TrafficLand;
	
};

#endif

