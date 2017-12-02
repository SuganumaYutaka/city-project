/*==============================================================================

    LandView.h - ���̎��������[���r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/
#ifndef _LAND_VIEW_H_
#define _LAND_VIEW_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class LandAttribute;
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
//�_�i�����_�j
class LandView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	LandView( GameObject* pGameObject);
	void Uninit( void);

	void SetAttribute( LandAttribute* attribute);

	void UpdateAttribute( void) { m_IsUpdatedAttribute = true;}

private:
	void Update(void);

	LandAttribute* m_Attribute;

	bool m_IsUpdatedAttribute;
};

#endif

