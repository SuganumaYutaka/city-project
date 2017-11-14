/*==============================================================================

    MeshFieldCollider.h - ���b�V���t�B�[���h�R���C�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/31
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "MeshFieldCollider.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "Transform.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define P_POS_X(P_WIDTH, NUM_FIELD_X) (0.0f + P_WIDTH * NUM_FIELD_X * -0.5f)		//�J�n�n�_��X���W
#define P_POS_Y (0.0f)																//�J�n�n�_��Y���W
#define P_POS_Z(P_HEIGHT, NUM_FIELD_Z) (0.0f + P_HEIGHT * NUM_FIELD_Z * 0.5f)		//�J�n�n�_��Z���W
#define P_WIDTH( ALL_WIDTH, NUM_FIELD_X)			( ALL_WIDTH / NUM_FIELD_X)		//�|���S���ꖇ������̕�
#define P_HEIGHT( ALL_HEIGHT, NUM_FIELD_Z)			( ALL_HEIGHT / NUM_FIELD_Z)		//�|���S���ꖇ������̍���

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* MeshFieldCollider::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshFieldCollider>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
MeshFieldCollider::MeshFieldCollider( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = pGameObject->GetComponent<Transform>();
	m_ColType = eColMeshField;
	m_IsTrigger = false;

	//�f�t�H���g�l�ݒ�
	m_nNumBlockX = 8;			//���̕�����
	m_nNumBlockZ = 8;			//�c�̕�����
	m_fWidth = 8.0f;			//��
	m_fHeight = 8.0f;			//����
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//�|���S��1��������̕�
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//�|���S��1��������̍���

	//���_�ݒ�
	m_pVtxPos = new Vector3[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
	for( int nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( int nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//���_���W�̐ݒ�
			m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				0.0f,
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
		}
	}

	//�|���S���ݒ�
	m_pPolygon = new POLYGON[ m_nNumBlockX * m_nNumBlockZ * 2];
	for (int nCntZ = 0; nCntZ < m_nNumBlockZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nNumBlockX; nCntX++)
		{
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 0] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 1] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 2] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].Normal = GetPolygonNormal( (nCntZ * m_nNumBlockX + nCntX) * 2 + 0);

			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 0] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 1] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 2] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].Normal = GetPolygonNormal( (nCntZ * m_nNumBlockX + nCntX) * 2 + 1);
		}
	}

}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void MeshFieldCollider::Uninit( void)
{
	Manager::GetCollisionManager()->ReleaseCollider( this);

	//���_���̉��
	delete[] m_pVtxPos;
	m_pVtxPos = NULL;

	//�|���S�����̉��
	delete[] m_pPolygon;
	m_pPolygon = NULL;
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void MeshFieldCollider::Update()
{
	
}

/*------------------------------------------------------------------------------
	�Փ�
	����
		Collider *pCollider			�Փ˂����R���C�_�[
------------------------------------------------------------------------------*/
void MeshFieldCollider::OnCollision( Collider *pCollider)
{
	
}

/*------------------------------------------------------------------------------
	�t�B�[���h��ݒ�
------------------------------------------------------------------------------*/
void MeshFieldCollider::SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�ݒ�
	m_nNumBlockX = X;
	m_nNumBlockZ = Z;
	m_fBlockWidth = BlockWidth;
	m_fBlockHeight = BlockHeight;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	
	//���_���̉��
	delete[] m_pVtxPos;
	m_pVtxPos = NULL;

	//�|���S�����̉��
	delete[] m_pPolygon;
	m_pPolygon = NULL;

	//���_�ݒ�
	m_pVtxPos = new Vector3[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
	for( int nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( int nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			if( pVertexHeight != NULL)
			{
				//���_���W�̐ݒ�
				m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
					m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
					pVertexHeight[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth],
					m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
			}
			else
			{
				//���_���W�̐ݒ�
				m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
					m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
					0.0f,
					m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
			}
		}
	}

	//�|���S���ݒ�
	m_pPolygon = new POLYGON[ m_nNumBlockX * m_nNumBlockZ * 2];
	for (int nCntZ = 0; nCntZ < m_nNumBlockZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nNumBlockX; nCntX++)
		{
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 0] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 1] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].nVtxIndex[ 2] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 0].Normal = GetPolygonNormal((nCntZ * m_nNumBlockX + nCntX) * 2 + 0);

			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 0] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 1] = ( nCntZ + 1) * ( m_nNumBlockX + 1) + nCntX + 1;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].nVtxIndex[ 2] = ( nCntZ + 0) * ( m_nNumBlockX + 1) + nCntX;
			m_pPolygon[ (nCntZ * m_nNumBlockX + nCntX) * 2 + 1].Normal = GetPolygonNormal((nCntZ * m_nNumBlockX + nCntX) * 2 + 1);
		}
	}
}

/*------------------------------------------------------------------------------
	�|���S���ꖇ������̕��E������ݒ�
------------------------------------------------------------------------------*/
void MeshFieldCollider::SetBlockSize( float Width, float Height)
{
	//�ݒ�
	m_fBlockWidth = Width;
	m_fBlockHeight = Height;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	
	//���_�ݒ�
	for( int nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( int nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//���_���W�̐ݒ�
			m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth].y,
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
		}
	}
}

/*------------------------------------------------------------------------------
	���E������ݒ�
------------------------------------------------------------------------------*/
void MeshFieldCollider::SetSize( float Width, float Height)
{
	//�ݒ�
	m_fWidth = Width;
	m_fHeight = Height;
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//�|���S��1��������̕�
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//�|���S��1��������̍���
	
	//���_�ݒ�
	for( int nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( int nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//���_���W�̐ݒ�
			m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth] = Vector3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				m_pVtxPos[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth].y,
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
		}
	}
}

/*------------------------------------------------------------------------------
	�������擾
------------------------------------------------------------------------------*/
float MeshFieldCollider::GetHeight( const Vector3& Pos)
{
	int nIndexPolygon = GetIndexPolygon( Pos);			//�|���S���̃C���f�b�N�X�ԍ�

	//Pos���t�B�[���h�ォ
	if (nIndexPolygon == -1)
	{
		return -5.0f;
	}

	//�@���擾
	Vector3 Normal = m_pPolygon[ nIndexPolygon].Normal;

	//�������Z�o
	Vector3 PosZero = m_pVtxPos[ m_pPolygon[ nIndexPolygon].nVtxIndex[ 0]];
	float Height = PosZero.y - (( Pos.x - PosZero.x) * Normal.x + ( Pos.z - PosZero.z) * Normal.z) / Normal.y;
	
	return Height;
}

/*------------------------------------------------------------------------------
	Pos���Əd�Ȃ�|���S���i�C���f�b�N�X�j���擾
------------------------------------------------------------------------------*/
int MeshFieldCollider::GetIndexPolygon( const Vector3& Pos)
{
	float PosX = Pos.x;
	float PosZ = Pos.z;
	int nBlockX, nBlockZ;

	PosX += m_fWidth * 0.5f;
	nBlockX = (int)( PosX / m_fBlockWidth);
	PosZ = m_fHeight * 0.5f - PosZ;
	nBlockZ = (int)( PosZ / m_fBlockHeight);

	//�t�B�[���h��̂Ƃ�
	//if (PosX >= 0 && nBlockX < m_nNumBlockX && PosZ >= 0 && nBlockZ < m_nNumBlockZ)
	//{
	//	//�O�ςɂ�锻��
	//	if (IsPointInPolygon((nBlockZ * m_nNumBlockX + nBlockX) * 2, Pos) == true)
	//	{
	//		return (nBlockZ * m_nNumBlockX + nBlockX) * 2;
	//	}
	//	//else if (IsPointInPolygon((nBlockZ * m_nNumBlockX + nBlockX) * 2 + 1, Pos) == true)
	//	else
	//	{
	//		return (nBlockZ * m_nNumBlockX + nBlockX) * 2 + 1;
	//	}
	//}

	for (int nCnt = 0; nCnt < m_nNumBlockX * m_nNumBlockZ * 2; nCnt++)
	{
		if (IsPointInPolygon(nCnt, Pos) == true)
		{
			return nCnt;
		}
	}

	return -1;
}

/*------------------------------------------------------------------------------
	����_���|���S�����ɂ��邩
------------------------------------------------------------------------------*/
bool MeshFieldCollider::IsPointInPolygon( int nIndex, const Vector3& Pos)
{
	Vector3 Vec01 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[1]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]];
	Vector3 Vec0P = Pos - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]];
	//Vec01.y = 0.0f;
	//Vec0P.y = 0.0f;
	Vector3 Cross0 = Vector3::Cross(Vec01, Vec0P);
	if (Cross0.y < 0.0f)
	{
		return false;
	}

	Vector3 Vec12 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[2]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[1]];
	Vector3 Vec1P = Pos - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[1]];
	//Vec12.y = 0.0f;
	//Vec1P.y = 0.0f;
	Vector3 Cross1 = Vector3::Cross(Vec12, Vec1P);
	if (Cross1.y < 0.0f)
	{
		return false;
	}
	
	Vector3 Vec20 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[2]];
	Vector3 Vec2P = Pos - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[2]];
	//Vec20.y = 0.0f;
	//Vec2P.y = 0.0f;
	Vector3 Cross2 = Vector3::Cross(Vec20, Vec2P);
	if (Cross2.y < 0.0f)
	{
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------
	�|���S���̖ʖ@�����擾
------------------------------------------------------------------------------*/
Vector3 MeshFieldCollider::GetPolygonNormal( int nIndex)
{
	Vector3 Vec1 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[1]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]];
	Vector3 Vec2 = m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[2]] - m_pVtxPos[ m_pPolygon[ nIndex].nVtxIndex[0]];
	
	return Vector3::Cross( Vec1, Vec2);
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void MeshFieldCollider::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	float* pVertexHeight = NULL;

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "NumBlockX")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockX = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "NumBlockZ")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockZ = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "Width")
		{
			text.ForwardPositionToNextWord();
			m_fWidth = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Height")
		{
			text.ForwardPositionToNextWord();
			m_fHeight = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockWidth")
		{
			text.ForwardPositionToNextWord();
			m_fBlockWidth = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockHeight")
		{
			text.ForwardPositionToNextWord();
			m_fBlockHeight = std::stof(text.GetWord());
		}

		else if (text.GetWord() == "VertexHeight")
		{
			pVertexHeight = new float[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
			for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
			{
				text.ForwardPositionToNextWord();
				pVertexHeight[ nCnt] = std::stof(text.GetWord());
			}
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

	SetField( m_nNumBlockX, m_nNumBlockZ, m_fBlockWidth, m_fBlockHeight, pVertexHeight);
}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void MeshFieldCollider::Save(Text& text)
{
	StartSave(text);

	text += "NumBlockX " + std::to_string(m_nNumBlockX) + ' ';
	text += "NumBlockZ " + std::to_string(m_nNumBlockZ) + ' ';
	text += "Width " + std::to_string(m_fWidth) + ' ';
	text += "Height " + std::to_string(m_fHeight) + ' ';
	text += "BlockWidth " + std::to_string(m_fBlockWidth) + ' ';
	text += "BlockHeight " + std::to_string(m_fBlockHeight) + '\n';
	text += "VertexHeight ";
	for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
	{
		text += std::to_string(m_pVtxPos[ nCnt].y) + ' ';
	}
	text += "\n";

	EndSave( text);
}