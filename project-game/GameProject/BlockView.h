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
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
//�_�i�����_�j
class BlockView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BlockView( GameObject* pGameObject);
	void Uninit( void);

	void SetAttribute( BlockAttribute* attribute);

	void UpdateAttribute( void) { m_IsUpdatedAttribute = true;}

private:
	void Update(void);

	BlockAttribute* m_Attribute;

	bool m_IsUpdatedAttribute;
};

#endif

