    					 /*-----------------------------------------------------+
						  |				       NebuPart.cpp						|
						  |														|
						  |	Va gérer le champ de particule de la nebuleuse de	|
						  |						la 1ere partie					|
						  |														|
						  | 	U2^PoPsy TeAm 2000								|
						  +-----------------------------------------------------*/

#include "stdafx.h"

#ifdef _DEBUG
	#define NB_PARTICUL	1500
#else
	#define NB_PARTICUL	3500
#endif

#define NEBU_RAYON	2400.f

#define BIGPART_X		 800.f
#define BIGPART_Y		 3895.f
#define BIGPART_Z		-2033.f
#define BIGPART_SIZE	 900.f

#define PART_SIZE_MIN  10.f
#define PART_SIZE_RND  25.f

#define PART_RFACT	0.4f
#define PART_VFACT	0.4f
#define PART_BFACT	0.5f


#define NB_RAYON 800

#define RAY_SIZE_MIN 250.f
#define RAY_SIZE_RND 350.f

#define RAY_LIFE_MIN	.9f*FRAMEPERSECOND
#define RAY_LIFE_RND	1.8f*FRAMEPERSECOND



//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------------+
//									|  Class OneNebuParticule	|
//									+---------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
OneNebuParticule::OneNebuParticule()
{
	PosKeyFileU3D	allKey[11]	;
	Ufloat			disttmp		;
	Ufloat			ydectmp		;

	Ufloat	dist = ( NEBU_RAYON /*- BIGPART_SIZE*.5f*/ ) * Random1()  + BIGPART_SIZE	;
	Ufloat	angle = 2.f * M_PI * Random1()												;
	dist = .6f*dist + NEBU_RAYON*0.4f * sin( angle * 2.f * M_PI )						;
	Ufloat  anglespeed = ((1.0f - dist / NEBU_RAYON )* 30.f + 10.f) * .0174532925f		;
	Ufloat	fact = 1.0f - dist / NEBU_RAYON												;

	//-------------------- calcul decalage en Y
	Ufloat ydec = dist															;
	if( ydec > 900.f )	ydec = 900.f											;
	ydec = 350.f * ( .5f + .5f*cos( M_PI * ydec / 900.f ) ) + 50.f * Random1() * ( 1.0f - dist /NEBU_RAYON )	;
	if( Random1()>.5f ) ydec = -ydec											;

	//------------------- calcul les déplacements
	for( U32 a=0; a<11; a++ )
	{
		allKey[a].k.Ini()	;

		if( a< 6 )
			allKey[a].k.time = (U32)( FRAMEPERSECOND* ( a*5.f + 2.f*Random1() - 1.f ) )	;
		else if( a== 6 )
			allKey[a].k.time = (U32)( FRAMEPERSECOND* ( 28.f + 1.f*Random1() ) )	;
		else if( a== 7 )
			allKey[a].k.time = (U32)( FRAMEPERSECOND* ( 29.f + 1.f*Random1() ) )	;
		else if( a== 8 )
			allKey[a].k.time = (U32)( FRAMEPERSECOND* ( 30.f + 1.f*Random1() ) )	;
		else if( a== 9 )
			allKey[a].k.time = (U32)( FRAMEPERSECOND* ( 31.f + 1.f*Random1() ) )	;
		else if( a== 10 )
			allKey[a].k.time = (U32)( FRAMEPERSECOND* ( 32.f + 1.f*Random1() ) )	;

		disttmp = dist + NEBU_RAYON*.05f * Random1() - NEBU_RAYON*.025f				;
		ydectmp = ydec + ydec * .1f * Random1() - ydec * .05f						;

		vec3_set( allKey[a].p, disttmp * cos( -angle ) + BIGPART_X , 
							   ydectmp + BIGPART_Y,
							   disttmp * sin( -angle ) + BIGPART_Z )	;

		switch( a )
		{
			case 7:
			case 8:
			case 9:
			case 10:
				angle += anglespeed*.6f /*+ .08f * Random1() - .04f*/	;
				dist *= .9												;
				break;

			default:
				angle += anglespeed + .14f * Random1() - .07f	;
				break;
		}
	}

	//-------------------- alloue et calcul la spline
	mouvement = new SplineU3D( allKey, 11 )	;

	//-------------------- calcul taille et couleur
	size = PART_SIZE_MIN + PART_SIZE_RND * Random1()																	;
	vec4_set( RVBA, PART_RFACT*(1.f-fact) + fact, PART_VFACT*(1.f-fact) + fact, PART_BFACT*(1.f-fact) + fact, 0.6f )	;
	vec3_mul( RVBA, RVBA, Random1()*(1.f-fact) + fact )																	;
}
//----------------------------------------------------------------------------------------------------------
OneNebuParticule::~OneNebuParticule()
{
	delete mouvement	;
}
//----------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|  Class NebuParticule	|
//									+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
Nebuparticule::Nebuparticule( LPCSTR nomTex, LPCSTR nomTexEx )
{
	TextureManager::SetRepertoire( "" )													;
	partTex = TextureManager::OpenJpg( nomTex, BILINEAR_TEXTURE|TRILINEAR_TEXTURE  )	;
	partTex->IncUser()																	;

	partMat	= new MaterialU3D( "partMat" )	;
	partMat->SetTexture1( partTex )			;
	partMat->SetTransp( MatTranspAddAlpha )	;
	partMat->SetShading( MatShadingFlat )	;
	partMat->Confirm()						;

	partBmp = new BitmapU3D( partMat )	;

	Explodeima = new UImage( nomTexEx )	;
	Explodeima->Disable()				;

	AllPart = new OneNebuParticule[NB_PARTICUL]	;	// tout les déplacements de chak particule

	centreX = 0.5f	;
	centreY = 0.5f	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
Nebuparticule::~Nebuparticule()
{
	delete [] AllPart					;
	delete Explodeima					;
	delete partBmp						;
	delete partMat						;
	TextureManager::SubUse( partTex )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// affiche grosse particule centrale
void Nebuparticule::AffBigPart()
{
	CoordU3D tmpc	;
	Ufloat	 size	;

	vec3_set( tmpc.trans, BIGPART_X, BIGPART_Y, BIGPART_Z )	;
	U3D3Monde3D->GetActualCamera()->ProjetUni( &tmpc, 0 )	;

	if( !(tmpc.ClipInfo&ClipProche) )
	{
		centreX = tmpc.ecran[0]	;
		centreY = tmpc.ecran[1]	;

		size = BIGPART_SIZE * tmpc.ecran[3] * .5f	;

		partMat->SetColor( 1.0f, 1.0f, 1.0f, .8f )		;

		partBmp->SetCoord( 0, tmpc.ecran[0] - size, tmpc.ecran[1] - size )	;
		partBmp->SetCoord( 1, tmpc.ecran[0] + size, tmpc.ecran[1] - size )	;
		partBmp->SetCoord( 2, tmpc.ecran[0] + size, tmpc.ecran[1] + size )	;
		partBmp->SetCoord( 3, tmpc.ecran[0] - size, tmpc.ecran[1] + size )	;

		partBmp->Affiche()	;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
// affiche le champ de particules
void Nebuparticule::AffAllPart( Ufloat step )
{
	CoordU3D			tmpc									;
	Ufloat				size									;
	Camera*				cam = U3D3Monde3D->GetActualCamera()	;
	OneNebuParticule	*tmpp = AllPart							;

	for( U32 a=0; a<NB_PARTICUL; a++,tmpp++)
	{
		tmpc.frame_process = -1		;
		tmpp->Move( step )			;
		tmpp->GetPos( tmpc.trans )	;
		cam->ProjetUni( &tmpc, 0 )	;

		if( !(tmpc.ClipInfo&ClipProche) )
		{
			size = tmpp->GetSize() * tmpc.ecran[3] * .5f	;

			partMat->SetColor( tmpp->GetColor() )	;

			partBmp->SetCoord( 0, tmpc.ecran[0] - size, tmpc.ecran[1] - size )	;
			partBmp->SetCoord( 1, tmpc.ecran[0] + size, tmpc.ecran[1] - size )	;
			partBmp->SetCoord( 2, tmpc.ecran[0] + size, tmpc.ecran[1] + size )	;
			partBmp->SetCoord( 3, tmpc.ecran[0] - size, tmpc.ecran[1] + size )	;

			partBmp->Affiche()	;
		}
	}

}
//----------------------------------------------------------------------------------------------------------------------------------------
void Nebuparticule::Explode( Ufloat time )
{
	time *= FRAMEPERSECOND	;

	Explodeima->Enable()								;
	Explodeima->SetTransp( MatTranspAdd )				;
	Explodeima->InterpoAlpha( 0.0f, 1.0f, time*.7f )	;
	Explodeima->InterpoZoom( 0.5f, 4.0f, time )			;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void Nebuparticule::UpdateFrame( Ufloat step )
{
	U3D3Pipeline->Begin( partMat )	;

		AffBigPart()		;
		AffAllPart( step )	;

	U3D3Pipeline->End(  partMat )	;

	Explodeima->SetPos( centreX, centreY )	;
	Explodeima->Affiche( step )				;
}
//----------------------------------------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|  Class OneNebuRayon	|
//									+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
OneNebuRayon::OneNebuRayon()
{
	Ini()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OneNebuRayon::Ini()
{
	vec3_set( vec, 2.f*Random1() -1.f, 1.f*Random1() -.5f, 2.f*Random1() -1.f ) 	;
	vec3_normalize( vec )															;

	dist = NEBU_RAYON * Random1() * 2.f + BIGPART_SIZE		;
	taille	= RAY_SIZE_MIN + 	Random1()*RAY_SIZE_RND		;

	totalTime = timeToGo = 	RAY_LIFE_MIN + Random1()*RAY_LIFE_RND	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OneNebuRayon::GetVal( Ufloat debut[3], Ufloat fin[3], Ufloat &intensity )
{
	Ufloat pos = timeToGo / totalTime ;

	vec3_mul( debut, vec, dist*pos )	;
	debut[0] += BIGPART_X				;
	debut[1] += BIGPART_Y				;
	debut[2] += BIGPART_Z				;


	vec3_mul( fin, vec, (dist+taille)*pos )	;
	fin[0] += BIGPART_X						;
	fin[1] += BIGPART_Y						;
	fin[2] += BIGPART_Z						;

	intensity = .7f - pos*pos*.7f	; 
}
//----------------------------------------------------------------------------------------------------------------------------------------






//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-------------------+
//									|  Class NebuRayon	|
//									+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
NebuRayon::NebuRayon( Ufloat tappear )
{
	AllRayon = new OneNebuRayon[ NB_RAYON ]	;

	TotalTime = TimeToGo = tappear*FRAMEPERSECOND	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
NebuRayon::~NebuRayon()
{
	delete [] AllRayon	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void NebuRayon::UpdateFrame( Ufloat step )
{
	OneNebuRayon*	tmp = AllRayon							;
	CoordU3D		pt1,pt2									;
	Camera*			cam = U3D3Monde3D->GetActualCamera()	;
	Ufloat			RVBA[2][4]								;
	Ufloat			intensity								;
	Ufloat			APintensity							;

	APintensity = 1.0f - TimeToGo / TotalTime	;

	vec4_set( RVBA[0], .5f, .5f, .5f, 1.0f )	;
	vec4_set( RVBA[1], .1f, .1f, .1f, 1.0f )	;

	U3D3Pipeline->Begin( MatTranspAddAlpha )	;

		for( U32 a=0; a<NB_RAYON; a++,tmp++)
		{
			if( !tmp->Move( step*APintensity )	)
				tmp->Ini()	;

			pt1.frame_process = -1								;
			pt2.frame_process = -1								;
			tmp->GetVal( pt1.trans, pt2.trans, intensity )	;

			vec4_set( RVBA[0], intensity, intensity, intensity, APintensity )				;
			vec4_set( RVBA[1], intensity*.2f, intensity*.2f, intensity*.2f, APintensity )	;

			U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, RVBA )	;
		}

	U3D3Pipeline->End(  NULL )	;


	TimeToGo -= step						;
	if( TimeToGo < 0.0f ) TimeToGo = 0.0f	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
