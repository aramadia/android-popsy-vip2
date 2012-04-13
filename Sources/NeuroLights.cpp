	   			 /*---------------------------------------------------------------------+
				  |				         NeuroLights.cpp								|
				  |																		|
				  |		c'est ici ke l'on vagérer l'affichage 2D des gnolies lights		|
				  |																		|
				  | 	U2^PoPsy TeAm 2000												|
				  +---------------------------------------------------------------------*/


#include "stdafx.h"


#define BIGLIGHT_SIZE		600.f
#define BIGRAYXLIGHT_SIZE	600.f

// definit la position des points suplémentaire pour les tests d'oclusions de la big lumière
#define TEST_OCLU_X 10.f/DEFAULT_SCREEN_X
#define TEST_OCLU_Y 10.f/DEFAULT_SCREEN_Y

#define LENS1_POS	2.f	
#define LENS2_POS	1.f	
#define LENS3_POS	3.f
#define LENS4_POS	-1.5f
#define LENS5_POS	-1.2f

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|  Class NeuroneLights	|
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
NeuroLights::NeuroLights( MondeU3D *monde )
{
	m_monde = monde												;
//	BigLight = (LightU3D*)monde->GetLight( "MainL" )->GetPtr()	;
	BigLight = (LightU3D*)monde->GetLight( "MainL" )			;

	Big = new UImage( "Datas/meshes/neurones/BFlare.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	Big->SetTransp( MatTranspAddAlpha )																;

	BigRay = new UImage( "Datas/meshes/neurones/FlarePart.jpg", BILINEAR_TEXTURE )	;
	BigRay->SetTransp( MatTranspAddAlpha )												;

	Lens[0] = new UImage( "Datas/meshes/neurones/lens1.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	Lens[1] = new UImage( "Datas/meshes/neurones/lens2.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	Lens[2] = new UImage( "Datas/meshes/neurones/lens3.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	Lens[3] = new UImage( "Datas/meshes/neurones/lens4.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	Lens[4] = new UImage( "Datas/meshes/neurones/lens5.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	Lens[0]->SetTransp( MatTranspAddAlpha )																;
	Lens[1]->SetTransp( MatTranspAddAlpha )																;
	Lens[2]->SetTransp( MatTranspAddAlpha )																;
	Lens[3]->SetTransp( MatTranspAddAlpha )																;
	Lens[4]->SetTransp( MatTranspAddAlpha )																;

	BigAlpha = 1.0f					;
	BigAlphaToGo = 1.0f				;
	for( U32 a=0; a<8; a++ )
	{
		BigRayAlpha[a] = 1.0f		;
		BigRayAlphaToGo[a] = 1.0f	;
	}

}
//----------------------------------------------------------------------------------------------------------
NeuroLights::~NeuroLights()
{
	delete Big		;
	delete BigRay	;
	delete Lens[0]	;
	delete Lens[1]	;
	delete Lens[2]	;
	delete Lens[3]	;
	delete Lens[4]	;
}
//----------------------------------------------------------------------------------------------------------
void NeuroLights::AffLensFlare( Ufloat laptime )
{
	U32			a									;
	Ufloat		puissance,tmpv[3],vx,vy				;
	Camera*		cam = m_monde->GetActualCamera()	;

	puissance = BigAlpha	;
	for( a=0; a<8; a++ )
		puissance += BigRayAlpha[a]	;

	puissance /= 9.f	;

	BigLight->GetPos( tmpv )						;	// recup position de la lumière
	vec3_sub( tmpv, cam->GetPos(), tmpv )			;
	vec3_normalize( tmpv )							;
	puissance *= -vec3_dot( tmpv, cam->GetVue() )	;

//	puissance = 1.0f	;

	if( puissance> 0.0f )
	{
		vx = .5f - Bigxpos	;	vy = .5f - Bigypos 	;
/*		Ufloat dist = Sqrt( vx*vx+ vy*vy )			;
		vx /= dist									;
		vy /= dist									;*/

		Lens[0]->SetAlpha( puissance )									;
		Lens[0]->SetZoom( puissance*.7f + .3f+.05f*Random1() )			;
		Lens[0]->SetPos( vx*LENS1_POS+Bigxpos, vy*LENS1_POS+Bigypos )	;
		Lens[0]->Affiche()												;
		
		Lens[1]->SetAlpha( puissance )									;
		Lens[1]->SetZoom( puissance*.5f + .5f+.05f*Random1() )			;
		Lens[1]->SetPos( vx*LENS2_POS+Bigxpos, vy*LENS2_POS+Bigypos )	;
		Lens[1]->Affiche()												;

		Lens[2]->SetAlpha( puissance )									;
		Lens[2]->SetZoom( puissance*.6f + .4f +.05f*Random1() )			;
		Lens[2]->SetPos( vx*LENS3_POS+Bigxpos, vy*LENS3_POS+Bigypos )	;
		Lens[2]->Affiche()												;

		Lens[3]->SetAlpha( puissance )									;
		Lens[3]->SetZoom( puissance*.5f + .5f +.05f*Random1() )			;
		Lens[3]->SetPos( vx*LENS4_POS+Bigxpos, vy*LENS4_POS+Bigypos )	;
		Lens[3]->Affiche()												;

		Lens[4]->SetAlpha( puissance )									;
		Lens[4]->SetZoom( puissance*.5f + .5f +.05f*Random1() )			;
		Lens[4]->SetPos( vx*LENS5_POS+Bigxpos, vy*LENS5_POS+Bigypos )	;
		Lens[4]->Affiche()												;
	}
}
//----------------------------------------------------------------------------------------------------------
void NeuroLights::AffBigLight( Ufloat LapTime )
{
	U32			a									;
	CoordU3D	tmpc								;
	Ufloat		tmpv[3], tmpv1[3],balpha=0.0f		;
	Camera*		cam = m_monde->GetActualCamera()	;

	BigLight->GetPos( tmpc.trans )							;	// recup position de la lumière
	U3D3Monde3D->GetActualCamera()->ProjetUni( &tmpc, 0 )	;	// calcul coord 2d
	Bigxpos	= tmpc.ecran[0]									;
	Bigypos	= tmpc.ecran[1]									;

	BigAlpha += ( BigAlphaToGo - BigAlpha ) / 2.f	;
	for( a=0; a<8; a++ )
	{
		BigRayAlpha[a] += ( BigRayAlphaToGo[a] - BigRayAlpha[a] ) / 3.f	;
		balpha = BigRayAlpha[a] / 8.f									;
	}


	if( !(tmpc.ClipInfo&ClipProche) )
	{
		cam->Cam2MondePos( tmpv1, tmpc.ecran )	;

		//---------------- check et affiche rayon gauche de la big light 
		vec3_eg( tmpv, tmpc.ecran )			;
		tmpv[0] -= TEST_OCLU_X				;
		cam->Cam2MondePos( tmpv1, tmpv )	;
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpv1 ) )
			BigRayAlphaToGo[0] = 1.0f	;
		else
			BigRayAlphaToGo[0] = 0.0f	;

		if( BigRayAlpha[0] > 0.05f )
		{
			BigRay->SetAngle( 0.0f )												;
			BigRay->SetAlpha( BigRayAlpha[0] )										;
			BigRay->SetXZoom( BigRayAlpha[0]*.5f + .5f  )							;
			BigRay->SetPos( tmpc.ecran[0] - BigRay->GetXSize()*.5f, tmpc.ecran[1] )	;

			BigRay->Affiche()	;
		}

		//---------------- check et affiche rayon droit de la big light 
		vec3_eg( tmpv, tmpc.ecran )			;
		tmpv[0] += TEST_OCLU_X				;
		cam->Cam2MondePos( tmpv1, tmpv )	;
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpv1 ) )
			BigRayAlphaToGo[1] = 1.0f	;
		else
			BigRayAlphaToGo[1] = 0.0f	;

		if( BigRayAlpha[1] > 0.05f )
		{
			BigRay->SetAngle( Pi() )												;
			BigRay->SetAlpha( BigRayAlpha[1] )										;
			BigRay->SetXZoom( BigRayAlpha[1]*.5f + .5f  )							;
			BigRay->SetPos( tmpc.ecran[0] + BigRay->GetXSize()*.5f, tmpc.ecran[1] )	;

			BigRay->Affiche()	;
		}

		//---------------- check et affiche rayon haut de la big light 
		vec3_eg( tmpv, tmpc.ecran )			;
		tmpv[1] -= TEST_OCLU_Y				;
		cam->Cam2MondePos( tmpv1, tmpv )	;
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpv1 ) )
			BigRayAlphaToGo[2] = 1.0f	;
		else
			BigRayAlphaToGo[2] = 0.0f	;

		if( BigRayAlpha[2] > 0.05f )
		{
			BigRay->SetAngle( -Pi()*.5f )												;
			BigRay->SetAlpha( BigRayAlpha[2] )											;
			BigRay->SetXZoom( BigRayAlpha[2]*.5f + .5f  )								;
			BigRay->SetPos( tmpc.ecran[0] , tmpc.ecran[1] - BigRay->GetXSize()*.5f )	;

			BigRay->Affiche()	;
		}

		//---------------- check et affiche rayon bas de la big light 
		vec3_eg( tmpv, tmpc.ecran )			;
		tmpv[1] += TEST_OCLU_Y				;
		cam->Cam2MondePos( tmpv1, tmpv )	;
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpv1 ) )
			BigRayAlphaToGo[3] = 1.0f	;
		else
			BigRayAlphaToGo[3] = 0.0f	;

		if( BigRayAlpha[3] > 0.05f )
		{
			BigRay->SetAngle( Pi()*.5f )												;
			BigRay->SetAlpha( BigRayAlpha[3] )											;
			BigRay->SetXZoom( BigRayAlpha[3]*.5f + .5f  )								;
			BigRay->SetPos( tmpc.ecran[0] , tmpc.ecran[1] + BigRay->GetXSize()*.5f )	;

			BigRay->Affiche()	;
		}

		//---------------- check et affiche rayon haut gauche de la big light 
		vec3_eg( tmpv, tmpc.ecran )			;
		tmpv[0] -= TEST_OCLU_X*.5f			;
		tmpv[1] -= TEST_OCLU_Y*.5f			;
		cam->Cam2MondePos( tmpv1, tmpv )	;
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpv1 ) )
			BigRayAlphaToGo[4] = 1.0f	;
		else
			BigRayAlphaToGo[4] = 0.0f	;

		if( BigRayAlpha[4] > 0.05f )
		{
			BigRay->SetAngle( -Pi()*.25f )																;
			BigRay->SetAlpha( BigRayAlpha[4] )															;
			BigRay->SetXZoom( 1.0f  )																	;
			BigRay->SetPos( tmpc.ecran[0] - GetUniXpos( 45.25f) , tmpc.ecran[1] - GetUniXpos( 45.25f) )	;

			BigRay->Affiche()	;
		}

		//---------------- check et affiche rayon haut droit de la big light 
		vec3_eg( tmpv, tmpc.ecran )			;
		tmpv[0] += TEST_OCLU_X*.5f			;
		tmpv[1] -= TEST_OCLU_Y*.5f			;
		cam->Cam2MondePos( tmpv1, tmpv )	;
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpv1 ) )
			BigRayAlphaToGo[5] = 1.0f	;
		else
			BigRayAlphaToGo[5] = 0.0f	;

		if( BigRayAlpha[5] > 0.05f )
		{
			BigRay->SetAngle( Pi()*1.25f )																;
			BigRay->SetAlpha( BigRayAlpha[5] )															;
			BigRay->SetXZoom( 1.0f  )																	;
			BigRay->SetPos( tmpc.ecran[0] + GetUniXpos( 45.25f) , tmpc.ecran[1] - GetUniXpos( 45.25f) )	;

			BigRay->Affiche()	;
		}

		//---------------- check et affiche rayon bas gauche de la big light 
		vec3_eg( tmpv, tmpc.ecran )			;
		tmpv[0] -= TEST_OCLU_X*.5f			;
		tmpv[1] += TEST_OCLU_Y*.5f			;
		cam->Cam2MondePos( tmpv1, tmpv )	;
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpv1 ) )
			BigRayAlphaToGo[6] = 1.0f	;
		else
			BigRayAlphaToGo[6] = 0.0f	;

		if( BigRayAlpha[6] > 0.05f )
		{
			BigRay->SetAngle( Pi()*.25f )																;
			BigRay->SetAlpha( BigRayAlpha[6] )															;
			BigRay->SetXZoom( 1.0f  )																	;
			BigRay->SetPos( tmpc.ecran[0] - GetUniXpos( 45.25f) , tmpc.ecran[1] + GetUniXpos( 45.25f) )	;

			BigRay->Affiche()	;
		}

		//---------------- check et affiche rayon bas droit de la big light 
		vec3_eg( tmpv, tmpc.ecran )			;
		tmpv[0] += TEST_OCLU_X*.5f			;
		tmpv[1] += TEST_OCLU_Y*.5f			;
		cam->Cam2MondePos( tmpv1, tmpv )	;
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpv1 ) )
			BigRayAlphaToGo[7] = 1.0f	;
		else
			BigRayAlphaToGo[7] = 0.0f	;

		if( BigRayAlpha[7] > 0.05f )
		{
			BigRay->SetAngle( -Pi()*1.25f )																;
			BigRay->SetAlpha( BigRayAlpha[7] )															;
			BigRay->SetXZoom( 1.0f  )																	;
			BigRay->SetPos( tmpc.ecran[0] + GetUniXpos( 45.25f) , tmpc.ecran[1] + GetUniXpos( 45.25f) )	;

			BigRay->Affiche()	;
		}

		//---------------- check et affiche rayon central de la big light 
		if( !m_monde->SegmentIntersecPoly( cam->GetPos(), tmpc.trans ) )
			BigAlphaToGo = 1.0f	;
		else
			BigAlphaToGo = 0.0f	;

		if( BigAlpha > 0.05f )
		{
			Big->SetAlpha( BigAlpha*.5f + .5f*balpha )	;
			Big->SetPos( tmpc.ecran[0], tmpc.ecran[1] )	;

			Big->Affiche()		;
		}

		AffLensFlare( LapTime )	;
	}
	else	// tout est caché alors op la
	{
		BigAlphaToGo = 0.0f	;
		for( a=0; a<8; a++ )
			BigRayAlphaToGo[a] = 0.0f	;
	}
}
//----------------------------------------------------------------------------------------------------------
void NeuroLights::Affiche( Ufloat lapTime )
{
	MondeU3D *save = SetActualMondeU3D( m_monde )	;	// c'est + sur on sait jamais

	AffBigLight( lapTime )	;

	SetActualMondeU3D( save )	;
}
//----------------------------------------------------------------------------------------------------------
