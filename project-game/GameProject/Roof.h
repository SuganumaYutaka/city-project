/*==============================================================================

    Roof.h - 建物の自動生成ー屋根
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _ROOF_H_
#define _ROOF_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class Renderer;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Roof
{
public:
	Roof( GameObject* buildingObject);
	void InitPlane( const Vector3& position, float rotation, const Vector3& size, std::string fileName = "data/TEXTURE/default.jpg");

	void UpdatePosition( const Vector3& position);
	void UpdateRotation( float rotation);
	void UpdateSize( const Vector3& size);
	
private:
	GameObject* m_RoofObject;
	Renderer* m_Renderer;

};

#endif

