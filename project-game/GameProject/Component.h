/*==============================================================================

    Component.h - �R���|�[�l���g�i���N���X�j
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/27
==============================================================================*/
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class Transform;
class Collider;

/*------------------------------------------------------------------------------
	�񋓌^��`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Component
{
public:
	Component( GameObject *pGameObject);
	~Component();

	GameObject *GetGameObject() { return m_pGameObject;}	//���L�ҏ���n��
	Transform *GetTransform() { return m_pTransform;}

	virtual void Update( void) {}
	virtual void Uninit( void) {}
	
	virtual void OnCollision( Collider *pCollider) {}
	//virtual void OnCollisionEnter( Collider *pCollider) {}
	//virtual void OnCollisionExit( Collider *pCollider) {}

	int m_nLayer;					//���C���[

	GameObject *m_pGameObject;		//���L�ҁi�e�j
	Transform *m_pTransform;		//���L�҂̈ʒu�E��]�E�X�P�[��

	void ReleaseReserve( void );
	static void ReleaseList( void);

	virtual void SetActive( bool bEnable) { m_bEnable = bEnable;}
	bool GetActive( void) const { return m_bEnable;}

protected:
	Component();
	bool m_bEnable;					//���s�\��
	
private:
	static std::list<Component*> m_listReleaseComponent;	//�������X�g
	bool m_bRelease;										//�����\��ς݂�
};

#endif

