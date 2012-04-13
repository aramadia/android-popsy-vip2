					 /*-----------------------------------------------------------------+
					  |						CouplePluieL.cpp							|
					  |																	|
					  |	la classe ki affiche de la pluie en ligne sur la scene en avant	|
					  |						et en arrière plan.							|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/


#include "stdafx.h"


#define POSX_MAX	250.f
#define POSX_DELTA	500.f

#define POSY_MAX	200.f
#define POSY_DELTA	400.f

#define POSZ_MAX	250.f
#define POSZ_DELTA	500.f

#define NB_GOUTTES	200

#define NB_VENT_AVANT		200
#define NB_VENT_DERRIERE	100


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class CouplePluieL	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GoutteL::GoutteL()
{
	vec3_set( dir, 0.0f, 0.0f, 0.0f )	;
	vec3_set( pos, 0.0f, 0.0f, 0.0f )	;
	size = 0.0f							;
	vitesse	 = 0.0f						;
	alpha = 0.0f						;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GoutteL::Genere( Ufloat d[3] )
{
	vec3_eg( dir, d )			;

	vec3_set( pos, POSX_MAX - POSX_DELTA*Random1(), POSY_MAX - POSY_DELTA*.05f*Random1(), POSZ_MAX - POSZ_DELTA*Random1() )	;
	size = 15.f + 25.f*Random1()																							;

	vitesse = 350.f + 295.f*Random1()	;

	alpha = .5f + .5f*Random1()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class VentParticul	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
VentParticul::VentParticul()
{
	vec3_set( dir, 0.0f, 0.0f, 0.0f )	;
	vec3_set( pos, 0.0f, 0.0f, 0.0f )	;
	size = 0.0f							;
	vitesse	 = 0.0f						;
	alpha = 0.0f						;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void VentParticul::Genere( Ufloat d[3] )
{
	vec3_eg( dir, d )			;

	vec3_set( pos, POSX_MAX - POSX_DELTA*Random1(), POSY_MAX - POSY_DELTA*.05f*Random1(), POSZ_MAX - POSZ_DELTA*Random1() )	;
	size = 5.f + 5.f*Random1()																							;

	vitesse = 350.f + 295.f*Random1()	;

	alpha = .5f + .5f*Random1()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|   Class PluieL	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
PluieL::PluieL()
{
	//-------------------- ini et load l'image d'une particul !
	TextureManager::SetRepertoire( "" )																				;
	partTex = TextureManager::OpenJpg( "Datas/meshes/Couple/VentP.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE  )	;
	partTex->IncUser()																								;

	partMat	= new MaterialU3D( "partMat" )	;
	partMat->SetTexture1( partTex )			;
	partMat->SetTransp( MatTranspAddAlpha )	;
	partMat->SetShading( MatShadingFlat )	;
	partMat->Confirm()						;

	partBmp = new BitmapU3D( partMat )	;


	vec3_set( dirGeneral, 0.2f, -1.0f, 0.1f ) 	;
	vec3_normalize( dirGeneral )				;

	
	Gouttes = new GoutteL[ NB_GOUTTES ]					;
	VentDerriere = new VentParticul[ NB_VENT_DERRIERE ]	;
	VentDevant = new VentParticul[ NB_VENT_AVANT ]		;

	for( U32 a=0; a<NB_GOUTTES; a++ )
		Gouttes[a].Genere( dirGeneral )	;
	for( U32 a=0; a<NB_VENT_AVANT; a++ )
		VentDevant[a].Genere( dirGeneral )	;
	for( U32 a=0; a<NB_VENT_DERRIERE; a++ )
		VentDerriere[a].Genere( dirGeneral )	;

}
//----------------------------------------------------------------------------------------------------------------------------------------
PluieL::~PluieL()
{
	//------------------ KIll image de la particul
	delete partBmp						;
	delete partMat						;
	TextureManager::SubUse( partTex )	;

	delete [] Gouttes		;
	delete [] VentDerriere	;
	delete [] VentDevant	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PluieL::AffOneParti( Ufloat pos[3], Ufloat Size, Ufloat alpha )
{
	Camera*		cam = U3D3Monde3D->GetActualCamera()	;
	CoordU3D	tmpc									;
	Ufloat		size									;

	vec3_eg( tmpc.trans, pos )	;
	cam->ProjetUni( &tmpc, 0 )	;

	if( !(tmpc.ClipInfo&ClipProche) )
	{
		size = Size * tmpc.ecran[3] * .5f	;
		Ufloat sizey = 8.f*size				;

		partMat->SetAlpha( alpha )	;

		partBmp->SetCoord( 0, tmpc.ecran[0] - size, tmpc.ecran[1] - sizey )	;
		partBmp->SetCoord( 1, tmpc.ecran[0] + size, tmpc.ecran[1] - sizey )	;
		partBmp->SetCoord( 2, tmpc.ecran[0] + size, tmpc.ecran[1] + sizey )	;
		partBmp->SetCoord( 3, tmpc.ecran[0] - size, tmpc.ecran[1] + sizey )	;

		partBmp->Affiche()	;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
Ufloat PluieL::CalcAlpha( Ufloat y )
{
	 Ufloat alpha = 1.0f	;

	if( y < POSY_MAX - POSY_DELTA*.9f )
		alpha = (y - (POSY_MAX - POSY_DELTA) ) / ( (POSY_MAX - POSY_DELTA*.9f) - (POSY_MAX - POSY_DELTA) )	;

	if( y > POSY_MAX - POSY_DELTA*.1f )	
		alpha = (POSY_MAX - y ) / (POSY_MAX - (POSY_MAX - POSY_DELTA*.1f))	;

	return alpha	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PluieL::AffVent( VentParticul* tmpv, Ufloat laptime, U32 nbPart )
{
	Ufloat			pos[3]	;

	//----------- affiche le vent en arriere plan
	U3D3Pipeline->Begin( partMat, FALSE, TRUE )	;
		for( U32 a=0; a<nbPart; a++,tmpv++ )
		{
			tmpv->Move( laptime )	;

			//--------- si particul sort du champ definit alors on en genere une nouvelle
			tmpv->GetPos( pos )	;
			if( pos[1] < POSY_MAX - POSY_DELTA )
			{
				tmpv->Genere( dirGeneral )	;
				tmpv->GetPos( pos )			;	// reprend la position
			}

			Ufloat alpha = tmpv->GetAlpha()*CalcAlpha( pos[1] )	;
			
			AffOneParti( pos, tmpv->GetSize(), alpha )	;
		}
	U3D3Pipeline->End( partMat )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PluieL::UpdateFrameBackGround( Ufloat laptime )
{
	AffVent( VentDerriere, laptime, NB_VENT_DERRIERE )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PluieL::UpdateFrameForeGround( Ufloat laptime )
{
	CoordU3D		pt1,pt2									;
	Camera*			cam = U3D3Monde3D->GetActualCamera()	;
	Ufloat			RVBA[2][4]								;
	GoutteL*		tmpg									;

	//-------------- affiche les lignes ( ki représentent les gouttes d'eau )
	U3D3Pipeline->Begin( MatTranspAddAlpha, FALSE, TRUE )	;

		tmpg = Gouttes	;
		for( U32 a=0; a<NB_GOUTTES; a++,tmpg++ )
		{
			tmpg->Move( laptime )	;

			pt1.frame_process = -1					;
			pt2.frame_process = -1					;
			tmpg->GetPos1( pt1.trans ) 				;
			tmpg->GetPos2( pt2.trans ) 				;

			//-------------- calcul alpha de la goutte
			Ufloat alpha = 1.0f	;

/*			if( pt1.trans[1] < POSY_MAX - POSY_DELTA*.9f )
			{*/
				if( pt1.trans[1] < POSY_MAX - POSY_DELTA )
					tmpg->Genere( dirGeneral )	;
/*				else
					alpha = (pt1.trans[1] - (POSY_MAX - POSY_DELTA) ) / ( (POSY_MAX - POSY_DELTA*.9f) - (POSY_MAX - POSY_DELTA) )	;
			}

			if( pt1.trans[1] > POSY_MAX - POSY_DELTA*.1f )	alpha = (POSY_MAX - pt1.trans[1] ) / (POSY_MAX - (POSY_MAX - POSY_DELTA*.1f))	;*/

//			alpha = tmpg->GetAlpha()*CalcAlpha( pt1.trans[1] )		;

			vec4_set( RVBA[0], 0.4f, 0.4f, 0.4f, alpha )		;
			vec4_set( RVBA[1], 0.0f, 0.0f, 0.0f, alpha )		;

			U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, RVBA )	;
		}

	U3D3Pipeline->End( NULL )	;

//	AffVent( VentDevant, laptime, NB_VENT_AVANT )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
