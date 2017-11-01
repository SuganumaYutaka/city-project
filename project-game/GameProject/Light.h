/*==============================================================================

    Light.h - ���C�g
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/10
==============================================================================*/
#ifndef _LIGHT_H_
#define _LIGHT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�񋓌^��`
------------------------------------------------------------------------------*/
typedef enum
{
	eLightDirectional = 0,
	eLightPoint,
	eLightSpot,
	eNumLightType
}ELightType;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Light : public Component
{
public:
	Light( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	D3DXVECTOR4* GetDirection( void);
	D3DXVECTOR4* GetPosition( void);
	void SetType( ELightType Type) { m_Type = Type;}
	ELightType GetType( void){ return m_Type;}

	static std::list<Light*> GetList( void) { return m_listLight;}

protected:
	
	
	ELightType m_Type;			//�^�C�v
	static std::list<Light*> m_listLight;		//���C�g���X�g
	
};

#endif

