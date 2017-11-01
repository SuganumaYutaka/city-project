/*==============================================================================

    CollisionManager.h - �Փ˔���i�R���C�_�[�j�Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "CollisionManager.h"
#include "GameObject.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
	����
		HWND hWnd
		BOOL bWindow
------------------------------------------------------------------------------*/
CollisionManager::CollisionManager()
{
	
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
CollisionManager::~CollisionManager()
{
	
}

/*------------------------------------------------------------------------------
	�R���C�_�[���X�g�ɒǉ�
	����
		Collider *pCollider			�R���C�_�[
------------------------------------------------------------------------------*/
void CollisionManager::AddCollider( Collider *pCollider)
{
	m_listCollider.push_back( pCollider);
}

/*------------------------------------------------------------------------------
	�R���C�_�[���X�g�������
	����
		Collider *pCollider			�R���C�_�[
------------------------------------------------------------------------------*/
void CollisionManager::ReleaseCollider( Collider *pCollider)
{
	m_listReleaseCollider.push_back( pCollider);
}

/*------------------------------------------------------------------------------
	�R���C�_�[���X�g���\�[�g
------------------------------------------------------------------------------*/
void CollisionManager::Sort()
{

}

/*------------------------------------------------------------------------------
	�Փ˔���
------------------------------------------------------------------------------*/
void CollisionManager::Collision()
{
	//�R���C�_�[���X�g����폜
	for (auto Col : m_listReleaseCollider)
	{
		for (auto ite = m_listCollider.begin(); ite != m_listCollider.end(); ++ite)
		{
			if( *ite == Col)
			{
				m_listCollider.erase(ite);
				break;
			}
		}
	}
	m_listReleaseCollider.clear();

	//���I�I�u�W�F�N�g���m
	for( std::list< Collider *>::iterator Source = m_listCollider.begin(); Source != m_listCollider.end(); ++Source)
	{
		for( std::list< Collider *>::iterator Dest = Source; Dest != m_listCollider.end(); ++Dest)
		{
			if( *Source != NULL && *Dest != NULL && Source != Dest)
			{
				if( IsCollision( Source, Dest) == true)
				{
					//���݂��̃R���C�_�[�ɏՓ˂�m�点��
					dynamic_cast< Collider *>( *Source)->GetGameObject()->OnCollision( *Dest);
					dynamic_cast< Collider *>( *Dest)->GetGameObject()->OnCollision( *Source);
				}
			}
		}
	}

	//�ʒu���X�V
	for (auto Col : m_listCollider)
	{
		if (Col->GetActive() == true)
		{
			Col->m_OldPos = Col->m_pTransform->GetWorldPosition();
		}
	}
}

/*------------------------------------------------------------------------------
	�Փ˂��Ă��邩
	����
		std::list< Collider *>::iterator Source		�C�e���[�^�i�P�ځj
		std::list< Collider *>::iterator Dest		�C�e���[�^�i�Q�ځj
	�߂�l
		true		�Փ˂���
		false		�Փ˂Ȃ�
------------------------------------------------------------------------------*/
bool CollisionManager::IsCollision( std::list< Collider *>::iterator Source, std::list< Collider *>::iterator Dest)
{
	Collider *pSource = *Source;
	Collider *pDest = *Dest;

	if (pSource->GetActive() == false || pDest->GetActive() == false)
	{
		return false;
	}

	//�{�b�N�X�R���C�_�[���m
	if( pSource->GetColType() == eColBox && pDest->GetColType() == eColBox)
	{
		if (BoxBox((BoxCollider*)pSource, (BoxCollider*)pDest))
		{
			//�Փ˂���
			return true;
		}
	}

	//�{�b�N�X�ƃ��b�V���t�B�[���h
	else if (pSource->GetColType() == eColBox && pDest->GetColType() == eColMeshField)
	{
		if (BoxMeshField((BoxCollider*)pSource, (MeshFieldCollider*)pDest))
		{
			//�Փ˂���
			return true;
		}
	}

	else if (pSource->GetColType() == eColMeshField && pDest->GetColType() == eColBox)
	{
		if (BoxMeshField((BoxCollider*)pDest, (MeshFieldCollider*)pSource))
		{
			//�Փ˂���
			return true;
		}
	}

	//�Փ˂Ȃ�
	return false;
}

/*------------------------------------------------------------------------------
	�{�b�N�X�ƃ{�b�N�X
	����
		BoxCollider *pSource
		BoxCollider *pDest
	�߂�l
		true		�Փ˂���
		false		�Փ˂Ȃ�
------------------------------------------------------------------------------*/
bool CollisionManager::BoxBox( BoxCollider *pSource, BoxCollider *pDest)
{
	//�ϐ��錾
	Vector3 SourcePos = pSource->GetTransform()->GetWorldPosition();
	Vector3 SourceSize = pSource->GetSize() * 0.5f;
	Vector3 DestPos = pDest->GetTransform()->GetWorldPosition();
	Vector3 DestSize = pDest->GetSize() * 0.5f;

	//���S�ʒu�����炷
	D3DXVECTOR3 center;
	D3DXMATRIX mtxRot;
	center = pSource->GetCenter().ConvertToDX();
	D3DXMatrixRotationQuaternion( &mtxRot, &pSource->m_pTransform->GetWorldRotation());
	D3DXVec3TransformCoord( &center, &center, &mtxRot);
	SourcePos += Vector3::ConvertFromDX( center);
	center = pDest->GetCenter().ConvertToDX();
	D3DXMatrixRotationQuaternion( &mtxRot, &pDest->m_pTransform->GetWorldRotation());
	D3DXVec3TransformCoord( &center, &center, &mtxRot);
	DestPos += Vector3::ConvertFromDX( center);

	//�Փ˔���
	if( SourcePos.x + SourceSize.x > DestPos.x - DestSize.x && SourcePos.x - SourceSize.x < DestPos.x + DestSize.x &&
		SourcePos.y + SourceSize.y > DestPos.y - DestSize.y && SourcePos.y - SourceSize.y < DestPos.y + DestSize.y &&
		SourcePos.z + SourceSize.z > DestPos.z - DestSize.z && SourcePos.z - SourceSize.z < DestPos.z + DestSize.z)
	{
		//�I�u�W�F�N�g���͂�������
		//pSource���ÓI�I�u�W�F�N�g��pDest�����I�I�u�W�F�N�g
		if( pSource->m_IsTrigger == false && pDest->m_IsTrigger == true)
		{
			Vector3 Pos = pDest->m_pTransform->GetWorldPosition();

			//��O
			if ( SourcePos.z - SourceSize.z > pDest->m_OldPos.z + DestSize.z)
			{
				Pos.z = SourcePos.z - SourceSize.z - DestSize.z;
			}

			//��
			else if( SourcePos.z + SourceSize.z < pDest->m_OldPos.z - DestSize.z)
			{
				Pos.z = SourcePos.z + SourceSize.z + DestSize.z;
			}

			//��
			if ( SourcePos.x - SourceSize.x > pDest->m_OldPos.x + DestSize.x)
			{
				Pos.x = SourcePos.x - SourceSize.x - DestSize.x;
			}

			//�E
			else if( SourcePos.x + SourceSize.x < pDest->m_OldPos.x - DestSize.x)
			{
				Pos.x = SourcePos.x + SourceSize.x + DestSize.x;
			}

			//��
			if ( SourcePos.y - SourceSize.y > pDest->m_OldPos.y + DestSize.y)
			{
				Pos.y = SourcePos.y - SourceSize.y - DestSize.y;
			}

			//��
			else if( SourcePos.y + SourceSize.y < pDest->m_OldPos.y - DestSize.y)
			{
				Pos.y = SourcePos.y + SourceSize.y + DestSize.y;
			}

			Vector3 move = Pos - pDest->m_pTransform->GetWorldPosition();
			move *= 1.1f;
			pDest->m_pTransform->Move( move);

		}

		//pSource�����I�I�u�W�F�N�g��pDest���ÓI�I�u�W�F�N�g
		else if (pSource->m_IsTrigger == true && pDest->m_IsTrigger == false)
		{
			Vector3 Pos = pSource->m_pTransform->GetWorldPosition();

			//��O
			if ( DestPos.z - DestSize.z > pSource->m_OldPos.z + SourceSize.z)
			{
				Pos.z = DestPos.z - DestSize.z - SourceSize.z;
			}

			//��
			else if( DestPos.z + DestSize.z < pSource->m_OldPos.z - SourceSize.z)
			{
				Pos.z = DestPos.z + DestSize.z + SourceSize.z;
			}

			//��
			if ( DestPos.x - DestSize.x > pSource->m_OldPos.x + SourceSize.x)
			{
				Pos.x = DestPos.x - DestSize.x - SourceSize.x;
			}

			//�E
			else if( DestPos.x + DestSize.x < pSource->m_OldPos.x - SourceSize.x)
			{
				Pos.x = DestPos.x + DestSize.x + SourceSize.x;
			}

			//��
			if ( DestPos.y - DestSize.y > pSource->m_OldPos.y + SourceSize.y)
			{
				Pos.y = DestPos.y - DestSize.y - SourceSize.y;
			}

			//��
			else if( DestPos.y + DestSize.y < pSource->m_OldPos.y - SourceSize.y)
			{
				Pos.y = DestPos.y + DestSize.y + SourceSize.y;
			}

			Vector3 move = Pos - pSource->m_pTransform->GetWorldPosition();
			move *= 1.1f;
			pSource->m_pTransform->Move( move);
		}

		else
		{
			//���Ή�
			//assert(false);
		}

		//�Փ˂���
		return true;
	}

	//�Փ˂Ȃ�
	return false;
}

/*------------------------------------------------------------------------------
	�{�b�N�X�ƃ��b�V���t�B�[���h
	����
		BoxCollider *pBox
		MeshFieldCollider *pMeshField
	�߂�l
		true		�Փ˂���
		false		�Փ˂Ȃ�
------------------------------------------------------------------------------*/
bool CollisionManager::BoxMeshField(BoxCollider *pBox, MeshFieldCollider *pMeshField)
{
	Vector3 Pos = pBox->GetTransform()->GetWorldPosition();
	Pos.y -= pBox->GetSize().y * 0.5f;
	float Height = pMeshField->GetHeight(Pos);

	//���S�ʒu�����炷
	D3DXVECTOR3 center;
	D3DXMATRIX mtxRot;
	center = pBox->GetCenter().ConvertToDX();
	D3DXMatrixRotationQuaternion( &mtxRot, &pBox->m_pTransform->GetWorldRotation());
	D3DXVec3TransformCoord( &center, &center, &mtxRot);
	Pos += Vector3::ConvertFromDX( center);

	if (Pos.y < Height)
	{
		//��Ɉړ�
		if (pMeshField->m_IsTrigger == false)
		{
			pBox->m_pTransform->SetWorldPositionY( Height + pBox->GetSize().y * 0.5f - pBox->GetCenter().y);
		}

		//�Փ˂���
		return true;
	}

	//�Փ˂Ȃ�
	return false;
}

/*------------------------------------------------------------------------------
	�����Ɨ����̂̏Փ˔��菈���֐�
	����
	D3DXVECTOR3 posWorld	�����̎n�_(���[���h���)
	D3DXVECTOR3 dirWorld	�����̕���(���[���h���)
	D3DXVECTOR3 size		�����̂̑傫��
	D3DXMATRIX mtxWorld		�����̂̃��[���h���W�ϊ��s��
	float *fDist			�n�_���痧���̂܂ł̋����i�o�́j
	D3DXVECTOR3 *colPos		�Փ˔���ʒu�i�o�́j
	�߂�l
	bool
------------------------------------------------------------------------------*/
bool CollisionManager::CollisionRayBox( D3DXVECTOR3 posWorld, D3DXVECTOR3 dirWorld, D3DXVECTOR3 size, D3DXMATRIX mtxWorld, float &Dist, D3DXVECTOR3 *colPos)
{
	//�����𗧕��̂̈ʒu�ֈړ�
	D3DXMATRIX mtxInv;			//���[���h���W�ϊ��s��̋t�s��
	D3DXMatrixInverse( &mtxInv, 0, &mtxWorld);

	D3DXVECTOR3 posLocal, dirLocal;		//�����̃��[�J�����W�E����
	D3DXVec3TransformCoord( &posLocal, &posWorld, &mtxInv);
	mtxInv._41 = 0.0f;
	mtxInv._42 = 0.0f;
	mtxInv._43 = 0.0f;
	D3DXVec3TransformCoord( &dirLocal, &dirWorld, &mtxInv);

	//��������
	//�ϐ��錾
	float pos[ 3 ], dir[ 3 ], min[ 3 ], max[ 3 ];		//for���ŉ񂷗p
	memcpy( pos, &posLocal, sizeof( D3DXVECTOR3 ) );
	memcpy( dir, &dirLocal, sizeof( D3DXVECTOR3 ) );
	memcpy( min, &D3DXVECTOR3( -size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f), sizeof( D3DXVECTOR3));
	memcpy( max, &D3DXVECTOR3( size.x * 0.5f, size.y * 0.5f, size.z * 0.5f), sizeof( D3DXVECTOR3));

	//�ő�l�E�ŏ��l���
	float fDistMin = -FLT_MAX;	//�ł��߂�����
	float fDistMax = FLT_MAX;	//�ł���������
	
	//XYZ�̌�������
	for( int nCnt = 0; nCnt < 3; nCnt++)
	{
		if ( abs( dir[ nCnt]) < FLT_EPSILON)
		{
			if ( pos[ nCnt] < min[ nCnt] || pos[ nCnt] > max[ nCnt] )
			{
				//�Փ˂Ȃ�
				return false;
			}
		}
		else
		{
			//�X���u�Ƃ̋������Z�o
			float odd = 1.0f / dir[ nCnt];
			float t1 = ( min[ nCnt] - pos[ nCnt]) * odd;
			float t2 = ( max[ nCnt] - pos[ nCnt]) * odd;
			if ( t1 > t2)
			{
				float tmp = t1; t1 = t2; t2 = tmp;		//�ő�E�ŏ������ւ�
			}
	
			//�������X�V
			if ( t1 > fDistMin)
			{
				fDistMin = t1;
			}
			if ( t2 < fDistMax)
			{
				fDistMax = t2;
			}

			// �X���u�����`�F�b�N
			if ( fDistMin >= fDistMax )
			{
				//�Փ˂Ȃ�
				return false;
			}
		}
	}

	//�Փˈʒu�o��
	if( colPos)
	{
		if( Dist != NULL)
		{
			Dist = fDistMin;
		}
		if( colPos != NULL)
		{
			*colPos = posWorld + fDistMin * dirWorld;
		}
	}
	
	//�Փ˂���
	return true;
}


