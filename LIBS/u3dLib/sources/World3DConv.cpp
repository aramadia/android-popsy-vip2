    				 /*-----------------------------------------------------+
					  |				        World3DConv.cpp					|
					  |														|
					  |	  cette classe me permet de traiter + facilement    |
					  |	    les données a convertir entre des datas 3D 		|
					  |		 quelconques et le formet spécifik U3D		    |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"

#ifdef TOOLU3D

MondeU3DConv	*MondeU3DConv::ActualMonde	;


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-------------------------+
//									|    Class MondeU3DConv   |
//									+-------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
MondeU3DConv::MondeU3DConv()
{
	ActualMonde = this	;

	nbobj	= 0		;
	ObjList	= NULL	;

	nbcam	= 0		;
	CamList	= NULL	;

	nblight = 0			;
	LightList = NULL	;

	nbframe = 0	;
}
//----------------------------------------------------------------------------------------------------------
MondeU3DConv::~MondeU3DConv()
{
	ObjetU3DConv	*Otmp	;
	CamU3DConv		*Ctmp	;
	LightU3DConv	*Ltmp	;

	//--------------- Kill objets
	while( ObjList )
	{
		Otmp = ObjList			;
		ObjList = ObjList->suiv	;
		delete Otmp				;
	}

	//-------------- Kill les cameras
	while( CamList )
	{
		Ctmp = CamList			;
		CamList = CamList->suiv	;
		delete Ctmp				;
	}

	//-------------- Kill les lights
	while( LightList )
	{
		Ltmp = LightList			;
		LightList = LightList->suiv	;
		delete Ltmp					;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3DConv::AddElement( ObjetU3DConv *obj )
{
	obj->suiv = ObjList	;
	ObjList = obj		;
	nbobj++				;
}
//----------------------------------------------------------------------------------------------------------
// ajoute une mesh dans la base de donnée 
void MondeU3DConv::AddElement( mesh3ds *mesh )
{
	ObjetU3DConv	*tmp	;

	tmp = new ObjetU3DConv	;

	tmp->Conv3DS( mesh )	;	// convertit la mesh !
	AddElement( tmp )		;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3DConv::AddElement( CamU3DConv *cam )
{
	cam->suiv = CamList	;
	CamList = cam		;
	nbcam++				;
}
//----------------------------------------------------------------------------------------------------------
// ajoute une camera dans la base de donnée
void MondeU3DConv::AddElement(camera3ds *cam, kfcamera3ds *kfcam)
{
	CamU3DConv *tmp	;

	tmp = new CamU3DConv	;

	tmp->Conv3DS( cam )	;	// convertit la camera !
	if( kfcam )
		tmp->Conv3DS( kfcam )	;

	AddElement( tmp )	;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3DConv::AddElement(LightU3DConv *li)
{
	li->suiv = LightList	;
	LightList = li			;
	nblight++				;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3DConv::AddElement( light3ds *li )
{
	LightU3DConv *tmp	;

	tmp = new LightU3DConv	;

	tmp->Conv3DS( li )	;
	AddElement( tmp )	;
}
//----------------------------------------------------------------------------------------------------------
// enleve element de la liste
void MondeU3DConv::RemoveElement(ObjetU3DConv *obj)
{
	ObjetU3DConv *actua,*preced=NULL	;

	actua = ObjList	;
	while( actua )
	{
		if( obj==actua )	break	;
		preced = actua				;
		actua = actua->suiv			;
	}

	if( actua )		// dans le cas ou lon a trouvé
	{
		if( preced )
		{
			preced->suiv = actua->suiv	;
		}
		else
		{
			ObjList = actua->suiv	;
		}
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3DConv::ConvAnim( kfmesh3ds *anim )
{
	CString			cstmp	;
	ObjetU3DConv	*otmp	;

	ActualMonde = this	;

	cstmp = anim->name				;
	otmp = Get( (LPCSTR)cstmp )		;
	otmp->Conv3DS( anim )			;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3DConv::ConvAnim( kfomni3ds *li )
{
	CString			cstmp	;
	LightU3DConv	*ltmp	;


	ActualMonde = this	;

	cstmp = li->name					;
	ltmp = GetLight( (LPCSTR)cstmp )	;
	ltmp->Conv3DS( li )					;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3DConv::ConvAnim( kfspot3ds *li )
{
	CString			cstmp	;
	LightU3DConv	*ltmp	;

	ActualMonde = this	;

	cstmp = li->name					;
	ltmp = GetLight( (LPCSTR)cstmp )	;
	ltmp->Conv3DS( li )					;
}
//----------------------------------------------------------------------------------------------------------
ObjetU3DConv *MondeU3DConv::Get( LPCSTR cs )
{
	ActualMonde = this	;

	ObjetU3DConv *tmp = ObjList	;

	while( tmp )
	{
		if( strcmp( tmp->GetNom(), (LPCSTR)cs )==0 )
			return tmp	;
		tmp = tmp->suiv	;
	}

	return NULL	;

/*	ObjetU3DConv *tmpO	;
	ObjetU3DConv *tmpO1	;

	tmpO = ObjList	;
	while( tmpO )
	{

		if( strcmp( tmpO->GetNom(), cs )==0 )
		{
			return tmpO	;
		}
		else	// recherche aussi parmi les fils de l'objet
		{
			tmpO1 = tmpO->FindChild( cs )	;
			if( tmpO1 ) return tmpO1		;
		}
		
		tmpO = tmpO->suiv	;
	}

	return NULL	;*/
}
//----------------------------------------------------------------------------------------------------------
LightU3DConv *MondeU3DConv::GetLight( LPCSTR cs )
{
	ActualMonde = this	;

	LightU3DConv	*li = LightList	;

	while( li )
	{
		if( strcmp( li->GetNom(), cs )==0 )
			return li	;
		li = li->suiv	;
	}

	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie le nombre de poly en du monde
// TODO : a faiireeeeeeeeeee
U32	MondeU3DConv::GetNBPolygon()
{
	return 0	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie le nombre de vertex en du monde
// TODO : a faiireeeeeeeeeee
U32	MondeU3DConv::GetNBVertices()
{
	return 0	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie le nombre de coordonnées en du monde
// TODO : a faiireeeeeeeeeee
U32	MondeU3DConv::GetNBCoord()
{
	return 0	;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3DConv::Serialize( CArchive &ar )
{
	ObjetU3DConv	*Otmp = ObjList		;
	CamU3DConv		*Ctmp = CamList		;
	LightU3DConv	*Ltmp = LightList	;

	ActualMonde = this	;

	if( ar.IsStoring()	)
	{
		ar << CHUNK_WOLRD	;

		ar << nbframe	;

		//-------------- Sauve couleur ambient
		ar << AmbientColor[0]	;
		ar << AmbientColor[1]	;
		ar << AmbientColor[2]	;

		//-------------- Sauve tout les objets
		while( Otmp )
		{
			if( Otmp->IsBase() )	// n'ecrit ke les objet ki n'ont pas de parents car le serialize de l'objet s'occupe des fils 
			{
				ar << CHUNK_OBJ			;
				Otmp->Serialize( ar )	;
			}
			Otmp = Otmp->suiv		;
		}

		//------------- Sauve toutes les cameras
		while( Ctmp )
		{
			ar << CHUNK_CAMERA		;
			Ctmp->Serialize( ar )	;
			Ctmp = Ctmp->suiv		;
		}

		//------------ Sauve toutes les lumières
		while( Ltmp )
		{
			ar << CHUNK_LIGHT		;
			Ltmp->Serialize( ar )	;
			Ltmp = Ltmp->suiv		;
		}

		ar << CHUNK_WOLRD_END	;
	}
}
//----------------------------------------------------------------------------------------------------------
#endif
