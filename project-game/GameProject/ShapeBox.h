/*==============================================================================

    ShapeBox.h - 建物の自動生成ーボックス
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/8
==============================================================================*/
#ifndef _SHAPE_BOX_H_
#define _SHAPE_BOX_H_

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
class ShapeBox : public Shape
{
public:
	ShapeBox( GameObject* buildingObject);
	~ShapeBox() override;

	void Init( const Vector3& position, float rotation, const Vector3& size, BuildingRule* rule);

	void Move( const Vector3& value) override;
	void Rotate( float value) override;
	void Scale( const Vector3& value) override;
	
private:
	Vector3 m_Position;
	float m_Rotation;
	Vector3 m_Size;
};

#endif

