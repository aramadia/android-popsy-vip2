    				 /*-----------------------------------------------------+
					  |				        CameraConv.cpp					|
					  |														|
					  |	  cette classe me permet de traiter + facilement    |
					  |	    les données a convertir entre des datas 3D 		|
					  |		 quelconques et le formet spécifik U3D		    |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"

#ifdef TOOLU3D
//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|    Class CameraConv   |
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
CamU3DConv::CamU3DConv()
{
	nom = ""		;

	vec3_set( pos, 0.0f, 0.0f, 0.0f )		;
	vec3_set( targ, 0.0f, 0.0f, 100.0f )	;
	roll = 0.0f 							;
	fov = 1.57f								;

	npkeys	= 0		;	// nb pos key
	posK	= NULL	;

	nfkeys	= 0		;	// nb fov key
	fovK	= NULL	;

	nrkeys	= 0		;	// nb roll key
	rollK	= NULL	;

	ntkeys	= 0		;	// nb target key
	targK	= NULL	;

	suiv	= NULL	;
}
//----------------------------------------------------------------------------------------------------------
CamU3DConv::~CamU3DConv()
{
	if( posK ) delete [] posK	;
	if( fovK ) delete [] fovK	;
	if( targK ) delete [] targK	;
	if( rollK ) delete [] rollK	;
}
//----------------------------------------------------------------------------------------------------------
void KHeadereg( KeyHeaderU3D &kdest, keyheader3ds &ksrc )
{
	kdest.time			= ksrc.time			;
	kdest.tension		= ksrc.tension		;
	kdest.continuity	= ksrc.continuity	;
	kdest.bias			= ksrc.bias			;
	kdest.easeto		= ksrc.easeto		;
	kdest.easefrom		= ksrc.easefrom		;
}
//----------------------------------------------------------------------------------------------------------
void CamU3DConv::Conv3DS( camera3ds *cam )
{
	nom = cam->name ;

	vec3_set( pos, cam->position.x, cam->position.z, cam->position.y )	;
	vec3_set( targ, cam->target.x, cam->target.z, cam->target.y )		;
	fov =  cam->fov														;
	roll = .0174532925f*cam->roll										;	// converti en radian
}
//----------------------------------------------------------------------------------------------------------
void CamU3DConv::Conv3DS( kfcamera3ds *cam )
{
	U32 a	;

	npkeys = (U16)cam->npkeys	;
	nrkeys = (U16)cam->nrkeys	;
	nfkeys = (U16)cam->nfkeys	;
	ntkeys = (U16)cam->ntkeys	;

	posK	= new PosKeyFileU3D[ npkeys ]	;
	rollK	= new UniKeyFileU3D[ nrkeys ]	;
	fovK	= new UniKeyFileU3D[ nfkeys ]	;
	targK	= new PosKeyFileU3D[ ntkeys ]	;

	//------- copie toutes les positions 	
	for(a=0; a<npkeys; a++)
	{
		vec3_set( posK[a].p, cam->pos[a].x, cam->pos[a].z, cam->pos[a].y )	;
		KHeadereg( posK[a].k, cam->pkeys[a] )								;
	}


	//------ copie tout les rolls
	for(a=0; a<nrkeys; a++)
	{
		rollK[a].u = .0174532925f*cam->roll[a]	;	// converti en radian
		KHeadereg( rollK[a].k, cam->rkeys[a] )	;
	}

	//------ copie tout les fovs
	for(a=0; a<nfkeys; a++)
	{
		fovK[a].u = cam->fov[a]					;
		KHeadereg( fovK[a].k, cam->fkeys[a] )	;
	}

	//----- copie tout les targets
	for(a=0; a<ntkeys; a++)
	{
		vec3_set( targK[a].p, cam->tpos[a].x, cam->tpos[a].z, cam->tpos[a].y )	;
		KHeadereg( targK[a].k, cam->tkeys[a] )									;
	}
}
//----------------------------------------------------------------------------------------------------------
void CamU3DConv::Serialize( CArchive &ar )
{
	U32	a	;

	if( ar.IsStoring() )
	{
		ar << nom	;

		ar << pos[0]	;
		ar << pos[1]	;
		ar << pos[2]	;

		ar << targ[0]	;
		ar << targ[1]	;
		ar << targ[2]	;

		ar << roll	;

		ar << fov	;

		//-------------- sauve les positions
		ar << npkeys										;
		for( a=0; a<npkeys;	a++	)	posK[a].Serialize( ar )	;

		//-------------- sauve les fovs
		ar << nfkeys										;
		for( a=0; a<nfkeys;	a++	)	fovK[a].Serialize( ar )	;

		//-------------- sauve les rolls
		ar << nrkeys											;
		for( a=0; a<nrkeys;	a++	)	rollK[a].Serialize( ar )	;

		//-------------- sauve les targets
		ar << ntkeys											;
		for( a=0; a<ntkeys;	a++	)	targK[a].Serialize( ar )	;
	}
}
//----------------------------------------------------------------------------------------------------------
#endif
