/*==============================================================================

    Light.h - ライト
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/10
==============================================================================*/
#ifndef _LIGHT_H_
#define _LIGHT_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	列挙型定義
------------------------------------------------------------------------------*/
typedef enum
{
	eLightDirectional = 0,
	eLightPoint,
	eLightSpot,
	eNumLightType
}ELightType;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Light : public Component
{
public:
	Light( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	D3DXVECTOR4* GetDirection( void);
	D3DXVECTOR4* GetPosition( void);
	void SetType( ELightType Type) { m_Type = Type;}
	ELightType GetType( void){ return m_Type;}

	static std::list<Light*> GetList( void) { return m_listLight;}

protected:
	
	
	ELightType m_Type;			//タイプ
	static std::list<Light*> m_listLight;		//ライトリスト
	
};

#endif

