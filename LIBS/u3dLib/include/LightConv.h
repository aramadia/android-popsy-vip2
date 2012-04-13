    				 /*-----------------------------------------------------+
					  |				        LightConv.h						|
					  |														|
					  |	  cette classe me permet de traiter + facilement    |
					  |	    les données a convertir entre des datas 3D 		|
					  |		 quelconques et le formet spécifik U3D		    |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifdef TOOLU3D
#ifndef _LIGHTCONV_U3D3_H
#define _LIGHTCONV_U3D3_H

class LightU3DConv{

	CString Nom		;
	U32		Type	;

	Ufloat Pos[3]	;

	Ufloat RVB[3]	;
	Ufloat Intensity;

	bool   Attenuation	;	// est-ce que l'attenuation est gérée ?
	Ufloat Inner		;
	Ufloat Outer		;

	//---------- donnée relative au spot !
	Ufloat	Targ[3]	;
	Ufloat	Hotspot	;
	Ufloat	Falloff	;

	U16				npkeys	;	// nb pos key
	PosKeyFileU3D	*posK	;

	U16				ntkeys	;	// nb target key
	PosKeyFileU3D	*targK	;

	U16				nckeys	;	// nb color key
	PosKeyFileU3D	*colorK	;

public:

	LightU3DConv	*suiv	;
	
	LightU3DConv()	;
	~LightU3DConv()	;

	LPCSTR GetNom() {return (LPCSTR)Nom;}

	void Conv3DS( light3ds *li )	;
	void Conv3DS( kfomni3ds *li )	;
	void Conv3DS( kfspot3ds *li )	;

	void Serialize( CArchive &ar )	;

};

#endif
#endif
