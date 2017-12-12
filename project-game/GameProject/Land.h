/*==============================================================================

    Land.h - �����̎��������[�y�n
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
class MeshPolygonRenderer;
class GameObject;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Land
{
public:
	Land( GameObject* buildingObject);
	void Init( const std::vector<Vector3>& vertices);

	const std::vector<Vector3>& GetVertices( void) { return m_Vertices;}

	Vector3 CulcCenterPosition( void);

	//TODO:�ʐς����߂�

private:
	std::vector<Vector3> m_Vertices;
	MeshPolygonRenderer* m_Renderer;

};

#endif

