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
	static Component* Create( GameObject* gameObject);

	Light( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	D3DXVECTOR4* GetDirection( void);
	D3DXVECTOR4* GetPosition( void);
	void SetType( ELightType Type) { m_Type = Type;}
	ELightType GetType( void){ return m_Type;}

	static std::list<Light*> GetList( void) { return m_listLight;}

	virtual void Save( Text& text);
	virtual void Load( Text& text);

	D3DXVECTOR4* GetAmbient( void) { return &m_Ambient;}
	D3DXVECTOR4* GetDiffuse( void) { return &m_Diffuse;}
	D3DXVECTOR4* GetSpecular( void) { return &m_Specular;}
	D3DXVECTOR4* GetEmissive( void) { return &m_Emissive;}
	
	void SetAmbient( float r, float g, float b, float a) { m_Ambient = D3DXVECTOR4( r, g, b, a);}
	void SetDiffuse( float r, float g, float b, float a) { m_Diffuse = D3DXVECTOR4( r, g, b, a);}
	void SetSpecular( float r, float g, float b, float a) { m_Specular = D3DXVECTOR4( r, g, b, a);}
	void SetEmissive( float r, float g, float b, float a) { m_Emissive = D3DXVECTOR4( r, g, b, a);}

protected:
	D3DXVECTOR4 m_Ambient;
	D3DXVECTOR4 m_Diffuse;
	D3DXVECTOR4 m_Specular;
	D3DXVECTOR4 m_Emissive;

	static const D3DXVECTOR4 DefAmb;
	static const D3DXVECTOR4 DefDif;
	static const D3DXVECTOR4 DefSpe;
	static const D3DXVECTOR4 DefEmi;
	
	ELightType m_Type;			//�^�C�v
	static std::list<Light*> m_listLight;		//���C�g���X�g
	
};

#endif

