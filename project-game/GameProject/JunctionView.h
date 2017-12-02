/*==============================================================================

    JunctionView.h - ���̎��������[�����_�r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/
#ifndef _JUNCTION_VIEW_H_
#define _JUNCTION_VIEW_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class JunctionAttribute;
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
//�_�i�����_�j
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

