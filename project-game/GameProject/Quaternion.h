/*==============================================================================

    Quaternion.h - クォータニオン演算補助
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/9
==============================================================================*/
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Quaternion
{
public:
	static D3DXQUATERNION LookAt(Vector3 sourcePoint, Vector3 destPoint)
	{
		D3DXQUATERNION quat;
		Vector3 forwardVector = Vector3::Normalize(destPoint - sourcePoint);

		float dot = Vector3::Dot(Vector3( 0.0f, 0.0f, 1.0f), forwardVector);

		if (abs(dot - (-1.0f)) < 0.000001f)
		{
			D3DXVECTOR3 vec( 0, 1, 0);
			D3DXQuaternionRotationAxis( &quat, &vec, D3DX_PI);
			return quat;
		}
		if (abs(dot - (1.0f)) < 0.000001f)
		{
			D3DXQuaternionIdentity( &quat);
			return quat;
		}

		float rotAngle = (float)acos(dot);
		Vector3 rotAxis = Vector3::Cross(Vector3( 0.0f, 0.0f, 1.0f), forwardVector);
		rotAxis = rotAxis.Normalize();
		return CreateFromAxisAngle(rotAxis, rotAngle);
	}

	static D3DXQUATERNION CreateFromAxisAngle(Vector3 axis, float angle)
	{
		float halfAngle = angle * 0.5f;
		float s = (float)sin(halfAngle);
		D3DXQUATERNION q;
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		q.w = (float)cos(halfAngle);
		return q;
	}
};

#endif




