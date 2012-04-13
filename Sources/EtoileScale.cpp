	   				 /*-------------------------------------------------------------------------+
					  |							EtoileScale.cpp									|
					  |																			|
					  |	affiche l'objet ki se deforme en Scale Additif pour faire une sorte 	|
					  |							de vibration 3d									|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#include "stdafx.h"

#define MAX_SCALE	2.0f
#define INC_SCALE	2.0f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class EtoileScale	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
EtoileScale::EtoileScale( MondeU3D* monde )
{
	Monde = monde	;

	for( Monde->IniObjIterator(); Monde->GetObjFromIterator(); Monde->IncObjIterator() )
	{
		if( strcmp( Monde->GetObjFromIterator()->GetNom(), "zarb" ) == 0 )
		{
			obj = Monde->GetObjFromIterator()	;
			break								;
		}
	}

	scale = 1.0f					;
	scale1 = (MAX_SCALE-1.0f)*.5f	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void EtoileScale::UpdateFrame( Ufloat laptime )
{
	Mat3x4		matrice		;
	TextureU3D*	saveTex2	;
	TextureU3D*	saveTex1	;
	U32			saveTransp	;

	scale += laptime*INC_SCALE				;
	if( scale >MAX_SCALE )	scale = 1.0f	;
	scale1 += laptime*INC_SCALE*.5f			;
	if( scale1 >MAX_SCALE )	scale1 = 1.0f	;


	//----------- sauve certains parametres du material
	saveTex1 = obj->Ptab[0].m->GetTex1()		;
	saveTex2 = obj->Ptab[0].m->GetTex2()		;
	saveTransp = obj->Ptab[0].m->GetTrTex1()	;

	//-------- met new material
	obj->Ptab[0].m->SetTexture2( NULL )					;
	obj->Ptab[0].m->SetTransp( MatTranspAddAlpha)		;
	obj->Ptab[0].m->SetAlpha( 1.0f - scale/MAX_SCALE )	;
	obj->Ptab[0].m->Confirm()							;

	//---------- transforme et affiche l'objet !
	matrice = obj->GetObjToWorld()	;
	matrice *= scale				;
//	matrice *= 1.25f				;
	Monde->IncTick()				;
	obj->Transform( matrice )		;
	obj->CalcPolyNormales( TRUE )	;
	obj->MakeCulling()				;
	obj->Affiche()					;

	U3D3Pipeline->Affiche()	;		// affiches tout les polys !
	U3D3Pipeline->Release()	;		// detruits tout les polys temporaires


	//--------- transforme et affiche le second objet
	obj->Ptab[0].m->SetAlpha( 1.0f - scale1/MAX_SCALE )	;
	matrice = obj->GetObjToWorld()	;
	matrice *= scale1				;
//	matrice *= 1.5f					;
	Monde->IncTick()				;
	obj->Transform( matrice )		;
	obj->CalcPolyNormales( TRUE )	;
	obj->MakeCulling()				;
	obj->Affiche()					;


	U3D3Pipeline->Affiche()	;		// affiches tout les polys !
	U3D3Pipeline->Release()	;		// detruits tout les polys temporaires*/

	//---------- restore le material	
	obj->Ptab[0].m->SetTexture1( saveTex1 )		;
	obj->Ptab[0].m->SetTexture2( saveTex2 )		;
	obj->Ptab[0].m->SetTransp( saveTransp )		;
	obj->Ptab[0].m->SetAlpha( 1.0f )			;
	obj->Ptab[0].m->Confirm()					;
}
//----------------------------------------------------------------------------------------------------------------------------------------

