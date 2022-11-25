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
/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class ShapeBox : public Shape
{
public:
	ShapeBox( GameObject* buildingObject);
	~ShapeBox() override;

	void Init( const Vector3& position, float rotation, const Vector3& size);
	void CreateWalls( void);

	void Move( const Vector3& value) override;
	void Rotate( float value) override;
	void ScaleUpDown( const Vector3& value) override;
	void ScaleRate( float rate) override;
	void ScaleRate( const Vector3& rate) override;
	void ConfirmShape( void) override;
	bool CollisionPoint( const Vector3& point) override;
	
private:
	Vector3 m_Size;
};

#endif

