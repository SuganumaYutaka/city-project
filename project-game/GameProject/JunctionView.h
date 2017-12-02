/*==============================================================================

    JunctionView.h - 町の自動生成ー交差点ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/
#ifndef _JUNCTION_VIEW_H_
#define _JUNCTION_VIEW_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class JunctionAttribute;
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
//点（交差点）
class JunctionView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	JunctionView( GameObject* pGameObject);
	void Uninit( void);

	void SetAttribute( JunctionAttribute* attribute);

	void UpdateAttribute( void) { m_IsUpdatedAttribute = true;}

private:
	void Update(void);

	JunctionAttribute* m_Attribute;
	Polygon3DRenderer* m_Renderer;

	bool m_IsUpdatedAttribute;
};

#endif

