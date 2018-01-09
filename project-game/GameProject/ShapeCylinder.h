#pragma once
/*==============================================================================

    ShapeCylinder.h - 建物の自動生成ーシリンダー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/9
==============================================================================*/
#ifndef _SHAPE_CYLINDER_H_
#define _SHAPE_CYLINDER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Shape.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class Roof;
class Wall;
class BuildingRule;

/*------------------------------------------------------------------------------
	クラス定義
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

