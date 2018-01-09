#pragma once
/*==============================================================================

    ShapeCylinder.h - �����̎��������[�V�����_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/9
==============================================================================*/
#ifndef _SHAPE_CYLINDER_H_
#define _SHAPE_CYLINDER_H_

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
class BuildingRule;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class ShapeCylinder : public Shape
{
public:
	ShapeCylinder( GameObject* buildingObject);
	~ShapeCylinder() override;

	void Init( const Vector3& position, float rotation, float height, float radius, BuildingRule* rule);
	void CreateWalls( void);

	void Move( const Vector3& value) override;
	void Rotate( float value) override;
	void ScaleUpDown( const Vector3& value) override;
	void ScaleRate( float rate) override;
	void ScaleRate( const Vector3& rate) override;
	void ConfirmShape( void) override;
	
private:
	float m_Height;
	float m_Radius;

};

#endif

