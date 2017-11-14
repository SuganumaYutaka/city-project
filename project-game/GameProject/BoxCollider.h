/*==============================================================================

    BoxCollider.h - �{�b�N�X�R���C�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Collider.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class MeshBoxRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BoxCollider : public Collider
{
public:
	static Component* Create( GameObject* gameObject);

	BoxCollider( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void OnCollision( Collider *pCollider);
	virtual void SetActive( bool bEnable);

	const Vector3& GetCenter() const { return m_Center;}
	const Vector3& GetSize() const { return m_Size;}

	void SetCenter( const Vector3& Center);
	void SetSize( const Vector3& Size);

	virtual void Save( Text& text);
	virtual void Load( Text& text);

private:
	Vector3 m_Center;				//���S�ʒu�iTransform�̈ʒu����̑��ΓI�Ȉʒu�j
	Vector3 m_Size;					//�傫��

#ifdef _DEBUG
	MeshBoxRenderer *m_pRenderer;	//�����_���[
#endif // _DEBUG


};

#endif

