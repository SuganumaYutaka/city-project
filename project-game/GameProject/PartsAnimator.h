/*==============================================================================

    PartsAnimator.h - �p�[�c�A�j���[�V����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/6
==============================================================================*/
#ifndef _PARTS_ANIMATOR_H_
#define _PARTS_ANIMATOR_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "GameObject.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class PartsAnimator : public Component
{
public:
	PartsAnimator( GameObject *pGameObject);
	void Uninit( void);

	void Load( const char *pFileName);
	void Update( void);

	void SetNextMotion( int nNextMotion, int nNextKey);
	int GetNowMotion( void) { return m_nNowMotion;}
	bool IsEndMotion(void);

private:
	typedef struct
	{
		Vector3 Position;		//�ʒu
		Vector3 Rotation;		//�p�x
	}KEY;

	typedef struct
	{
		int nFrame;					//�t���[����
		std::vector<KEY> vecKey;	//�L�[�f�[�^�i�p�[�c�������I�Ɋm�ہj
	}KEYSET;

	typedef struct
	{
		int nLoop;						//���[�v�t���O
		int nNumKey;					//�L�[��
		std::vector<KEYSET> vecKeySet;	//�L�[�ݒ�f�[�^
	}MOTION;

	std::vector<GameObject*> vecParts;		//�p�[�c�f�[�^�i�p�[�c�������I�m�ہj
	std::vector<Vector3> vecOfsetPos;		//�I�t�Z�b�g�i�ʒu�j
	std::vector<Vector3> vecOfsetRot;		//�I�t�Z�b�g�i�p�x�j
	std::vector<MOTION> vecMotion;			//���[�V�����f�[�^�i���[�V�����������I�m�ہj
	int m_nNowMotion;							//���݂̃��[�V����
	int m_nNextMotion;						//���̃��[�V����
	int m_nNowKey;							//���݂̃L�[
	int m_nNextKey;							//���̃L�[
	int m_nCntFrame;							//�t���[���J�E���^
	int m_nChangeFrame;						//�L�[�ύX�܂ł̃t���[����
	
	void SetFirstMotion();
	void ChangeMotion(int nNextMotion, int nNextKey);
};

#endif

