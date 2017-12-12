/*==============================================================================

    Building.h - ���̎��������[����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/
#ifndef _BUILDING_H_
#define _BUILDING_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Vector3.h"
#include <vector>

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Face;
}
class BuildingView;
class GameObject;
class BlockAttribute;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Building
{
private:
	HalfEdgeDataStructure::Face* m_Face;

public:
	Building() : m_Face( NULL){}
	~Building();

	void Init( BlockAttribute* attribute, GameObject* parent, const std::vector<Vector3> vertices);
	void Init( BlockAttribute* attribute, GameObject* parent, const Vector3& topLeft, const Vector3& topRight, const Vector3& bottomLeft, const Vector3& bottomRight);

	HalfEdgeDataStructure::Face* GetFace( void){ return m_Face;}
};

#endif

