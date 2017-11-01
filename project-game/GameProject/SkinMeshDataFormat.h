/*==============================================================================

    SkinMeshDataFormat.h - �X�L�����b�V���̃f�[�^�t�H�[�}�b�g
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/10/9
==============================================================================*/
#ifndef _SKIN_MESH_DATA_FORMAT_H_
#define _SKIN_MESH_DATA_FORMAT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Material;

/*------------------------------------------------------------------------------
	�\���̒�`
------------------------------------------------------------------------------*/
//�{�[�����
struct BornRefarence
{
	BornRefarence() : Index(0), Weight(0.0f) {}
	BornRefarence(unsigned char index, float weight) : Index(index), Weight(weight) {}
	unsigned char Index;		//�Q�Ƃ���{�[���̃C���f�b�N�X�ԍ�
	float Weight;				//�E�F�C�g�l
};

//���_���i�ʒu���W�ƃ{�[�����j
struct Point
{
	Point() : Position(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), nCntBornRefarence(0) {}
	Point(const D3DXVECTOR3& position) : Position(position), nCntBornRefarence(0) {}
	D3DXVECTOR3 Position;
	BornRefarence BornRefarence[4];
	int nCntBornRefarence;

	void SetBornRefarence(unsigned char index, float weight)
	{
		BornRefarence[nCntBornRefarence].Index = index;
		BornRefarence[nCntBornRefarence].Weight = weight;
		nCntBornRefarence++;
	}

};

//�A�j���[�V�������
struct Animation
{
	std::vector<std::vector<D3DXMATRIX>> vecMatrix;		//�A�j���[�V�������i�t���[�����~�N���X�^�[���j
	std::string Name;
	int AllFrame;
};

//�A�j���[�V�����N���b�v
struct AnimationClip
{
	std::string Name;
	int AllFrame;
};

#endif