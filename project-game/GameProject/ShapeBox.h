/*==============================================================================

    ShapeBox.h - �����̎��������[�{�b�N�X
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/8
==============================================================================*/
#ifndef _SHAPE_BOX_H_
#define _SHAPE_BOX_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shape.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class Roof;
class Wall;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class ShapeBox : public Shape
{
public:
	ShapeBox( GameObject* buildingObject);
	~ShapeBox();

	void Init( const Vector3& position, float rotation, const Vector3& size);

	void Move( const Vector3& value) override;
	void Rotate( float value) override;
	void Scaling( const Vector3& rate) override;
	
private:
	Vector3 m_Position;
	float m_Rotation;
	Vector3 m_Size;
};

#endif
