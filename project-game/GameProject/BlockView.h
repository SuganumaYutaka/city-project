/*==============================================================================

    BlockView.h - ���̎��������[���r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/
#ifndef _BLOCK_VIEW_H_
#define _BLOCK_VIEW_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class BlockAttribute;
class BlockModel;
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
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

