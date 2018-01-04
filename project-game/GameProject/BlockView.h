/*==============================================================================

    BlockView.h - 町の自動生成ー区画ビュー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/
#ifndef _BLOCK_VIEW_H_
#define _BLOCK_VIEW_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class BlockAttribute;
class BlockModel;
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class BlockView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BlockView( GameObject* pGameObject);
	void Uninit( void);

	void SetAttribute( BlockAttribute* attribute);
	BlockAttribute* GetAttribute( void) { return m_Attribute;}

	void UpdateAttribute( void);

private:
	void Update(void);

	BlockAttribute* m_Attribute;
	bool m_IsUpdatedAttribute;

	BlockModel* m_BlockModel;
};

#endif

