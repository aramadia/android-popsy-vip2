    				 /*-----------------------------------------------------+
					  |				        CameraConv.h					|
					  |														|
					  |	  cette classe me permet de traiter + facilement    |
					  |	    les données a convertir entre des datas 3D 		|
					  |		 quelconques et le formet spécifik U3D		    |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifdef TOOLU3D
#ifndef _CAMERACONV_U3D3_H
#define _CAMERACONV_U3D3_H


//==================

class CamU3DConv{

	CString nom		;
	Ufloat	pos[3]	;
	Ufloat	targ[3]	;
	Ufloat	roll	;
	Ufloat	fov		;

	U16				npkeys	;	// nb pos key
	PosKeyFileU3D	*posK	;

	U16				nfkeys	;	// nb fov key
	UniKeyFileU3D	*fovK	;

	U16				nrkeys	;	// nb roll key
	UniKeyFileU3D	*rollK	;

	U16				ntkeys	;	// nb target key
	PosKeyFileU3D	*targK	;

public:

	CamU3DConv	*suiv	;
	
	CamU3DConv()	;
	~CamU3DConv()	;

	void Conv3DS( camera3ds *cam )		;
	void Conv3DS( kfcamera3ds *cam )	;

	void Serialize( CArchive &ar )	;
};

void KHeadereg( KeyHeaderU3D &kdest, keyheader3ds &ksrc )	; // fonction de conversion

#endif
#endif
