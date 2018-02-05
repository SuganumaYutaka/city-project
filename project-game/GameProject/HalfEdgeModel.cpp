/*==============================================================================

    HalfEdgeModel.cpp - �n�[�t�G�b�W�\���[���f��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "HalfEdgeModel.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Rule.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Model::~Model()
{
	for (auto face : m_Faces)
	{
		delete face;
	}
	m_Faces.clear();
	m_Faces.shrink_to_fit();

	for (auto edge : m_Edges)
	{
		delete edge;
	}
	m_Edges.clear();
	m_Edges.shrink_to_fit();
	
	for (auto vertex : m_Vertices)
	{
		delete vertex;
	}
	m_Vertices.clear();
	m_Vertices.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	�_�𐶐�
------------------------------------------------------------------------------*/
Vertex* Model::CreateVertex(const Vector3& position)
{
	return new Vertex( this, position);
}

/*------------------------------------------------------------------------------
	�ӂ𐶐�
------------------------------------------------------------------------------*/
Edge* Model::CreateEdge( Vertex* start, Vertex* end)
{
	return new Edge( this, start, end);
}

/*------------------------------------------------------------------------------
	�ʂ𐶐�
------------------------------------------------------------------------------*/
Face* Model::CreateFace( HalfEdge* he)
{
	return new Face( this, he);
}

/*------------------------------------------------------------------------------
	�͂��߂Ă̓_�𐶐�
------------------------------------------------------------------------------*/
bool Model::CreateFirstVertex(const Vector3& position)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//�_�𐶐�
	CreateVertex( position);
	
	return true;
}

/*------------------------------------------------------------------------------
	�͂��߂Ă̕ӂ𐶐�
------------------------------------------------------------------------------*/
bool Model::CreateFirstEdge(const Vector3& startPosition, const Vector3& endPosition)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//�_�𐶐�
	auto start = CreateVertex( startPosition);
	auto end = CreateVertex( endPosition);

	//�ӂ𐶐�
	auto edge = CreateEdge( start, end);
	
	//�_�ɕӂ�ݒ�
	start->RegisterEdge( edge);
	end->RegisterEdge( edge);

	//�������̍X�V
	start->GetAttribute()->Update();
	end->GetAttribute()->Update();
	edge->GetAttribute()->Update();

	return true;
}

/*------------------------------------------------------------------------------
	�͂��߂Ă̖ʂ𐶐�
------------------------------------------------------------------------------*/
bool Model::CreateFirstFace(
	const Vector3& topLeftPosition, const Vector3& topRightPosition,
	const Vector3& bottomLeftPosition, const Vector3& bottomRightPosition)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//�_�𐶐�
	auto topLeft = CreateVertex( topLeftPosition);
	auto topRight = CreateVertex( topRightPosition);
	auto bottomLeft = CreateVertex( bottomLeftPosition);
	auto bottomRight = CreateVertex( bottomRightPosition);
	

	//�ӂ𐶐�
	auto top = CreateEdge( topLeft, topRight);
	auto bottom = CreateEdge( bottomRight, bottomLeft);
	auto left = CreateEdge( bottomLeft, topLeft);
	auto right = CreateEdge( topRight, bottomRight);

	//�_�ɕӂ�ݒ�
	topLeft->RegisterEdge( top);
	topLeft->RegisterEdge( left);
	topRight->RegisterEdge( top);
	topRight->RegisterEdge( right);
	bottomLeft->RegisterEdge( bottom);
	bottomLeft->RegisterEdge( left);
	bottomRight->RegisterEdge( bottom);
	bottomRight->RegisterEdge( right);

	//�n�[�t�G�b�W�𐶐�
	auto heTop = new HalfEdge( this, topRight);
	auto heRight = new HalfEdge( this, bottomRight);
	auto heBottom = new HalfEdge( this, bottomLeft);
	auto heLeft = new HalfEdge( this, topLeft);

	//�ӂɃn�[�t�G�b�W��ݒ�
	top->SetRight( heTop);
	right->SetRight( heRight);
	bottom->SetRight( heBottom);
	left->SetRight( heLeft);

	//�n�[�t�G�b�W�ɕӂ�next��ݒ�
	heTop->SetEdge( top);
	heRight->SetEdge( right);
	heBottom->SetEdge( bottom);
	heLeft->SetEdge( left);
	heTop->SetNext( heRight);
	heRight->SetNext( heBottom);
	heBottom->SetNext( heLeft);
	heLeft->SetNext( heTop);

	//�ʂ𐶐�
	auto face = CreateFace( heTop);

	//�n�[�t�G�b�W�ɖʂ�ݒ�
	heTop->SetFace( face);
	heRight->SetFace( face);
	heBottom->SetFace( face);
	heLeft->SetFace( face);

	//�������̍X�V
	topLeft->GetAttribute()->Update();
	topRight->GetAttribute()->Update();
	bottomLeft->GetAttribute()->Update();
	bottomRight->GetAttribute()->Update();
	top->GetAttribute()->Update();
	right->GetAttribute()->Update();
	bottom->GetAttribute()->Update();
	left->GetAttribute()->Update();
	face->GetAttribute()->Update();
	
	return true;
}

/*------------------------------------------------------------------------------
	���_���擾
------------------------------------------------------------------------------*/
Vertex* Model::GetVertex(int id)
{
	if (m_Vertices.size() <= id)
	{
		return NULL;
	}

	return m_Vertices[ id];
}

/*------------------------------------------------------------------------------
	���_��ID���擾
------------------------------------------------------------------------------*/
int Model::GetVertexID(Vertex* vertex)
{
	int size = m_Vertices.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Vertices[i] == vertex)
		{
			return i;
		}
	}

	//�����ł��Ȃ�
	return -1;
}

/*------------------------------------------------------------------------------
	�o�^������
------------------------------------------------------------------------------*/
bool Model::UnregisterVertex( Vertex* vertex)
{
	int size = m_Vertices.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Vertices[i] == vertex)
		{
			m_Vertices[i] = NULL;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	�ӂ��擾
------------------------------------------------------------------------------*/
Edge* Model::GetEdge(int id)
{
	if (m_Vertices.size() <= id)
	{
		return NULL;
	}

	return m_Edges[ id];
}

/*------------------------------------------------------------------------------
	�ӂ�ID���擾
------------------------------------------------------------------------------*/
int Model::GetEdgeID( Edge* edge)
{
	int size = m_Edges.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Edges[i] == edge)
		{
			return i;
		}
	}

	//�����ł��Ȃ�
	return -1;
}

/*------------------------------------------------------------------------------
	�o�^������
------------------------------------------------------------------------------*/
bool Model::UnregisterEdge( Edge* edge)
{
	int size = m_Edges.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Edges[i] == edge)
		{
			m_Edges[i] = NULL;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	�ʂ��擾
------------------------------------------------------------------------------*/
Face* Model::GetFace(int id)
{
	if (m_Vertices.size() <= id)
	{
		return NULL;
	}

	return m_Faces[ id];
}

/*------------------------------------------------------------------------------
	�ʂ�ID���擾
------------------------------------------------------------------------------*/
int Model::GetFaceID( Face* face)
{
	int size = m_Faces.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Faces[i] == face)
		{
			return i;
		}
	}

	//�����ł��Ȃ�
	return -1;
}

/*------------------------------------------------------------------------------
	�o�^������
------------------------------------------------------------------------------*/
bool Model::UnregisterFace( Face* face)
{
	int size = m_Faces.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Faces[i] == face)
		{
			m_Faces[i] = NULL;
			return true;
		}
	}

	return false;
}
