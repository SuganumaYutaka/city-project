/*==============================================================================

    PartsAnimator.cpp - �p�[�c�A�j���[�V����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/6
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "PartsAnimator.h"
#include "GameObject.h"
#include "Transform.h"
#include "XModelRenderer.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define MAX_BUF (1024)
#define TIME_CHANGE (5)					//���[�V�����؂�ւ��ɂ�����t���[��

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* PartsAnimator::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<PartsAnimator>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
PartsAnimator::PartsAnimator( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	m_nNowMotion = 0;
	m_nNextMotion = 0;
	m_nNowKey = 0;
	m_nNextKey = 1;
	m_nCntFrame = 0;
	m_nChangeFrame = 0;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void PartsAnimator::Uninit( void)
{
	//���[�V�����f�[�^�̉��
	for (MOTION Motion : vecMotion)
	{
		//�L�[�Z�b�g�̉��
		for (KEYSET Keyset : Motion.vecKeySet)
		{
			//�L�[�̉��
			Keyset.vecKey.clear();
		}
		Motion.vecKeySet.clear();
	}
	vecMotion.clear();
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void PartsAnimator::Update(void)
{
	//���[�V�����ݒ�
	MOTION motNow = vecMotion[ m_nNowMotion];			//���݂̃��[�V�����f�[�^
	MOTION motNext = vecMotion[ m_nNextMotion];		//���̃��[�V�����f�[�^

	//�L�[�Z�b�g�ݒ�
	KEYSET keysetNow =	motNow.vecKeySet[ m_nNowKey];		//���݂̃L�[�Z�b�g�f�[�^
	KEYSET keysetNext =	motNext.vecKeySet[ m_nNextKey];		//���̃L�[�Z�b�g�f�[�^

	//�f�[�^����p�[�c�̈ʒu�E�p�x�X�V
	Vector3 Pos;
	D3DXQUATERNION OffsetRot;
	D3DXQUATERNION NowRot;
	D3DXQUATERNION NextRot;
	float hoge = (float)m_nCntFrame / (float)(m_nChangeFrame);
	int sizeParts = vecParts.size();
	for( int nCntParts = 0; nCntParts < sizeParts; nCntParts++)
	{
		//�ʒu
		//Pos = keysetNow.vecKey[ nCntParts].Position + ( keysetNext.vecKey[ nCntParts].Position - keysetNow.vecKey[ nCntParts].Position) * (float)m_nCntFrame / m_nChangeFrame;
		//vecParts[ nCntParts]->m_pTransform->SetLocalPosition( vecOfsetPos[ nCntParts] + Pos);

		//�p�x
		D3DXQuaternionRotationYawPitchRoll( &OffsetRot, vecOfsetRot[ nCntParts].y, vecOfsetRot[ nCntParts].x, vecOfsetRot[ nCntParts].z);
		D3DXQuaternionRotationYawPitchRoll( &NowRot, keysetNow.vecKey[ nCntParts].Rotation.y, keysetNow.vecKey[ nCntParts].Rotation.x, keysetNow.vecKey[ nCntParts].Rotation.z);
		D3DXQuaternionRotationYawPitchRoll( &NextRot, keysetNext.vecKey[ nCntParts].Rotation.y, keysetNext.vecKey[ nCntParts].Rotation.x, keysetNext.vecKey[ nCntParts].Rotation.z);
		D3DXQuaternionSlerp( &NowRot, &NowRot, &NextRot, (FLOAT)(hoge));
		OffsetRot = NowRot * OffsetRot;
		vecParts[ nCntParts]->m_pTransform->SetLocalRotation( OffsetRot);
	}

	//�t���[���̍X�V
	if( motNow.nLoop == 1 || m_nNextKey + 1 < motNow.nNumKey || m_nCntFrame < m_nChangeFrame - 1)
	{
		m_nCntFrame++;
	}

	//�L�[�̕ύX
	if( m_nCntFrame >= m_nChangeFrame)
	{
		m_nCntFrame = 0;
		if( motNow.nLoop == 1)
		{
			ChangeMotion( m_nNextMotion, ( m_nNextKey + 1) % (motNow.nNumKey));
		}
		else
		{
			ChangeMotion( m_nNextMotion, ( m_nNextKey + 1));
		}
	}
}

/*------------------------------------------------------------------------------
	�A�j���[�V�����f�[�^�����[�h
------------------------------------------------------------------------------*/
void PartsAnimator::LoadAnimator( std::string fileName)
{
	//�X�N���v�g�ǂݍ���
	FILE *fp;			//�t�@�C���|�C���^
	char aBuf[MAX_BUF];	//�ǂݎ��p
	int nCnt = 0;		//�J�E���^
	std::vector<std::string> vecFileName;		//�ǂݍ��ރt�@�C����
	int nNumModel = 0;
	int nNumParts = 0;
	
	//�t�@�C���I�[�v��
	fp =fopen( fileName.c_str(), "r");
	if( fp == NULL)
	{
		MessageBox( NULL, "model.cpp\n�X�N���v�g�̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK);
		fclose(fp);
		return;
	}

	//�X�N���v�g�t�@�C�����`�F�b�N(1)
	while( 1)
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(1)\n", "�G���[", MB_OK);
			fclose(fp);
			return;
		}
		//�R�����g
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "SCRIPT") == 0)
		{
			break;
		}
	}

	//���f����(2)
	while( 1)
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(2)\n", "�G���[", MB_OK);
			fclose(fp);
			return;
		}
		//�R�����g
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "NUM_MODEL") == 0)
		{
			fscanf( fp, "%s", aBuf);
			fscanf( fp, "%d", &nNumModel);		//���f����
			//fscanf( fp, "%s", aFileName[ nCnt]);
			break;
		}
	}

	//���f���t�@�C����(3)
	nCnt = 0;
	while( nCnt < nNumModel)		//���f������
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(3)\n", "�G���[", MB_OK);
			fclose(fp);
			return;
		}
		//�R�����g
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "MODEL_FILENAME") == 0)
		{
			fscanf( fp, "%s", aBuf);
			fscanf( fp, "%s", aBuf);
			vecFileName.push_back( aBuf);		//���f���̃t�@�C����

			nCnt++;		//���f���J�E���g�A�b�v
		}
	}

	//�L�����N�^�[���(4)
	while( strcmp( aBuf, "END_CHARACTERSET") != 0)
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(4)\n", "�G���[", MB_OK);
			fclose(fp);
			return;
		}
		//�R�����g
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "CHARACTERSET") == 0)
		{
			//�L�����N�^�[���(5)
			while( strcmp( aBuf, "END_CHARACTERSET") != 0)
			{
				if( fscanf( fp, "%s", aBuf) == EOF)
				{
					MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(5)\n", "�G���[", MB_OK);
					fclose(fp);
					return;
				}
				//�R�����g
				if( aBuf[ 0] == '#')
				{
					fgets( aBuf, MAX_BUF, fp);
				}
				//�p�[�c��
				if( strcmp( aBuf, "NUM_PARTS") == 0)
				{
					fscanf( fp, "%s", aBuf);
					fscanf( fp, "%d", &nNumParts);
					break;		//���[�v������
				}
			}

			//�p�[�c�𐶐�
			if( nNumParts <= 0)
			{
				MessageBox( NULL, "model.cpp\n�p�[�c����ݒ肵�Ă�������\n", "�G���[", MB_OK);
				fclose(fp);
				return;
			}
			for (int i = 0; i < nNumParts; i++)
			{
				GameObject* pObj = new GameObject( m_pGameObject);
				pObj->IsCreatedByOtherComponent = true;
				auto renderer = pObj->AddComponent<XModelRenderer>();
				renderer->IsCreatedByOtherComponent = true;
				vecParts.push_back( pObj);
			}

			//�p�[�c���(6)
			nCnt = 0;
			while( strcmp( aBuf, "END_CHARACTERSET") != 0 && nCnt <= nNumParts)
			{
				if( fscanf( fp, "%s", aBuf) == EOF)
				{
					MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(6)\n", "�G���[", MB_OK);
					fclose(fp);
					return;
				}
				//�R�����g
				if( aBuf[ 0] == '#')
				{
					fgets( aBuf, MAX_BUF, fp);
				}
				if( strcmp( aBuf, "PARTSSET") == 0)
				{
					while( strcmp( aBuf, "END_CHARACTERSET") != 0
						&& strcmp( aBuf, "END_PARTSSET") != 0)
					{
						if( fscanf( fp, "%s", aBuf) == EOF)
						{
							MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(6')\n", "�G���[", MB_OK);
							fclose(fp);
							return;
						}
						//�R�����g
						if( aBuf[ 0] == '#')
						{
							fgets( aBuf, MAX_BUF, fp);
						}
						//���f���̃C���f�b�N�X�ԍ�
						if( strcmp( aBuf, "INDEX") == 0)
						{
							int nModelIdx = 0;
							fscanf( fp, "%s", aBuf);
							fscanf( fp, "%d", &nModelIdx);

							//���f����ݒ�
							vecParts[nCnt]->GetComponent<XModelRenderer>()->LoadXModel( vecFileName[nModelIdx]);
						}
						//�e�̃C���f�b�N�X�ԍ�
						if( strcmp( aBuf, "PARENT") == 0)
						{
							int nParent;
							fscanf( fp, "%s", aBuf);
							fscanf( fp, "%d", &nParent);
							if( nParent >= 0)
							{
								vecParts[nCnt]->m_pTransform->SetParent( vecParts[nParent]->m_pTransform);	//�e�̐ݒ�
							}
						}
						//�ʒu
						if( strcmp( aBuf, "POS") == 0)
						{
							Vector3 pos;
							fscanf( fp, "%s", aBuf);
							fscanf( fp, "%f", &pos.x);
							fscanf( fp, "%f", &pos.y);
							fscanf( fp, "%f", &pos.z);
							vecOfsetPos.push_back(pos);
							vecParts[nCnt]->m_pTransform->SetLocalPosition( pos);
						}
						//�p�x
						if( strcmp( aBuf, "ROT") == 0)
						{
							Vector3 rot;
							fscanf( fp, "%s", aBuf);
							fscanf( fp, "%f", &rot.x);
							fscanf( fp, "%f", &rot.y);
							fscanf( fp, "%f", &rot.z);
							vecOfsetRot.push_back(rot);
							vecParts[nCnt]->m_pTransform->SetLocalRotationEuler( rot);
						}
					}
					nCnt++;		//�p�[�c�J�E���g�A�b�v
				}
			}
		}
	}

	//���[�V�������(7)
	nCnt = 0;
	while( strcmp( aBuf, "END_SCRIPT") != 0)
	{
		if( fscanf( fp, "%s", aBuf) == EOF)
		{
			MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(7)\n", "�G���[", MB_OK);
			fclose(fp);
			return;
		}
		//�R�����g
		if( aBuf[ 0] == '#')
		{
			fgets( aBuf, MAX_BUF, fp);
		}
		if( strcmp( aBuf, "MOTIONSET") == 0)
		{
			//���[�V�����ǉ�
			MOTION motion;
			
			//���[�V�������(8)
			while( strcmp( aBuf, "END_MOTIONSET") != 0)
			{
				if( fscanf( fp, "%s", aBuf) == EOF)
				{
					MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(8)\n", "�G���[", MB_OK);
					fclose(fp);
					return;
				}
				//�R�����g
				if( aBuf[ 0] == '#')
				{
					fgets( aBuf, MAX_BUF, fp);
				}
				//���[�v�t���O
				if( strcmp( aBuf, "LOOP") == 0)
				{
					fscanf( fp, "%s", aBuf);
					fscanf( fp, "%d", &motion.nLoop);

				}
				//�L�[��
				if( strcmp( aBuf, "NUM_KEY") == 0)
				{
					fscanf( fp, "%s", aBuf);
					fscanf( fp, "%d", &motion.nNumKey);

					//�L�[�Z�b�g����
					motion.vecKeySet.resize( motion.nNumKey);

					//�L�[�Z�b�g���(9)
					int nCntKeySet = 0;		//�L�[�Z�b�g�J�E���^
					while( strcmp( aBuf, "END_MOTIONSET") != 0)
					{
						if( fscanf( fp, "%s", aBuf) == EOF)
						{
							MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(9)\n", "�G���[", MB_OK);
							fclose(fp);
							return;
						}
						//�R�����g
						if( aBuf[ 0] == '#')
						{
							fgets( aBuf, MAX_BUF, fp);
						}
						if( strcmp( aBuf, "KEYSET") == 0)
						{
							//�L�[����
							motion.vecKeySet[ nCntKeySet].vecKey.resize( nNumParts);

							while( strcmp( aBuf, "END_KEYSET") != 0)
							{
								if( fscanf( fp, "%s", aBuf) == EOF)
								{
									MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(9)\n", "�G���[", MB_OK);
									fclose(fp);
									return;
								}
								//�R�����g
								if( aBuf[ 0] == '#')
								{
									fgets( aBuf, MAX_BUF, fp);
								}
								//�t���[����
								if( strcmp( aBuf, "FRAME") == 0)
								{
									fscanf( fp, "%s", aBuf);
									fscanf( fp, "%d", &motion.vecKeySet[nCntKeySet].nFrame);
									break;
								}
							}

							//�L�[�Z�b�g���(10)
							int nCntKey = 0;		//�L�[�J�E���^
							while( strcmp( aBuf, "END_KEYSET") != 0)
							{
								if( fscanf( fp, "%s", aBuf) == EOF)
								{
									MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(10)\n", "�G���[", MB_OK);
									fclose(fp);
									return;
								}
								//�R�����g
								if( aBuf[ 0] == '#')
								{
									fgets( aBuf, MAX_BUF, fp);
								}
								//�L�[���(11)
								if( strcmp( aBuf, "KEY") == 0)
								{
									while( strcmp( aBuf, "END_KEY") != 0)
									{
										if( fscanf( fp, "%s", aBuf) == EOF)
										{
											MessageBox( NULL, "model.cpp\n�X�N���v�g������������܂���(11)\n", "�G���[", MB_OK);
											fclose(fp);
											return;
										}
										//�R�����g
										if( aBuf[ 0] == '#')
										{
											fgets( aBuf, MAX_BUF, fp);
										}
										//�ʒu
										if( strcmp( aBuf, "POS") == 0)
										{
											Vector3 pos;
											fscanf( fp, "%s", aBuf);
											fscanf( fp, "%f", &pos.x);
											fscanf( fp, "%f", &pos.y);
											fscanf( fp, "%f", &pos.z);
											motion.vecKeySet[ nCntKeySet].vecKey[nCntKey].Position = pos;
										}
										//�p�x
										if( strcmp( aBuf, "ROT") == 0)
										{
											Vector3 rot;
											fscanf( fp, "%s", aBuf);
											fscanf( fp, "%f", &rot.x);
											fscanf( fp, "%f", &rot.y);
											fscanf( fp, "%f", &rot.z);
											motion.vecKeySet[ nCntKeySet].vecKey[nCntKey].Rotation = rot;
										}
									}

									nCntKey++;	//�L�[�J�E���g�A�b�v
								}
							}
							nCntKeySet++;		//�L�[�Z�b�g�J�E���g�A�b�v
						}
					}
				}
			}
			vecMotion.push_back(motion);
			nCnt++;		//���[�V�����J�E���g�A�b�v
		}
	}
	
	//�t�@�C���N���[�Y
	fclose(fp);

	//�����l���
	m_nChangeFrame = vecMotion[ m_nNowMotion].vecKeySet[ m_nNowKey].nFrame;

	//�p�x�C��
	vecOfsetRot[0].y += D3DX_PI;

	FileName = fileName;
}

/*------------------------------------------------------------------------------
	�ŏ��̃��[�V�����ݒ菈���֐�
------------------------------------------------------------------------------*/
void PartsAnimator::SetFirstMotion()
{
	//�f�[�^����p�[�c�̈ʒu�E�p�x�ݒ�
	int sizeParts = vecParts.size();
	for( int nCntParts = 0; nCntParts < sizeParts; nCntParts++)
	{
		vecParts[nCntParts]->m_pTransform->SetLocalPosition( vecMotion[ m_nNowMotion].vecKeySet[ m_nNowKey].vecKey[ nCntParts].Position);
		vecParts[nCntParts]->m_pTransform->SetLocalRotationEuler( vecMotion[ m_nNowMotion].vecKeySet[ m_nNowKey].vecKey[ nCntParts].Rotation);
	}
}

/*------------------------------------------------------------------------------
	���̃��[�V�����ݒ菈���֐�
------------------------------------------------------------------------------*/
void PartsAnimator::SetNextMotion( int nNextMotion, int nNextKey)
{
	//���݂̃��[�V�����A�L�[���X�V
	m_nNowMotion = m_nNextMotion;
	m_nNowKey = m_nNextKey;

	//���̃��[�V�����A�L�[��ݒ�
	m_nNextMotion = nNextMotion;
	m_nNextKey = nNextKey;

	//�t���[���ݒ�
	m_nChangeFrame = m_nCntFrame + TIME_CHANGE;
}

/*------------------------------------------------------------------------------
	���[�V�����؂�ւ������֐�
------------------------------------------------------------------------------*/
void PartsAnimator::ChangeMotion( int nNextMotion, int nNextKey)
{
	//���݂̃��[�V�����A�L�[���X�V
	m_nNowMotion = m_nNextMotion;
	m_nNowKey = m_nNextKey;

	//���̃��[�V�����A�L�[��ݒ�
	m_nNextMotion = nNextMotion;
	m_nNextKey = nNextKey;

	//�t���[�������Z�b�g
	m_nCntFrame = 0;
	m_nChangeFrame = vecMotion[ m_nNowMotion].vecKeySet[ m_nNowKey].nFrame;
}

/*------------------------------------------------------------------------------
	���[�V�������I��������
------------------------------------------------------------------------------*/
bool PartsAnimator::IsEndMotion(void)
{
	auto motNow = vecMotion[ m_nNowMotion];
	if( motNow.nLoop == 1 || m_nNextKey + 1 < motNow.nNumKey || m_nCntFrame < m_nChangeFrame - 1)
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void PartsAnimator::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "FileName")
		{
			text.ForwardPositionToNextWord();
			LoadAnimator( text.GetWord());
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void PartsAnimator::Save(Text& text)
{
	StartSave(text);

	text += "FileName " + FileName + '\n';

	EndSave( text);
}