					 /*-----------------------------------------------------------------+
					  |							Ghost2D.cpp								|
					  |																	|
					  |	pour gérer l'affichage de tout se ki est 2D sur cette scene		|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#include "stdafx.h"

#define TAPPEAR	1.0f

#define TEMPS_NORM 3.f
#define TDISAPPEAR 1.f

#define DUREE TEMPS_NORM+TDISAPPEAR+TAPPEAR

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|	 Class Ghost2D		|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
Ghost2D::Ghost2D()
{
	Rond[0] = new UImage( "Datas/meshes/Ghost/Rond.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	Rond[0]->SetTransp( MatTranspAddAlpha )															;
	Rond[0]->InterpoAlpha( 0.0f, 1.0f, TAPPEAR )													;
	Rond[0]->Disable()																				;

	for( U32 a=0; a<2; a++)
	{
		Rond[a+1] = new UImage	;
		*Rond[a+1] = *Rond[0]	;
	}


	BarreV[0] = new UImage( "Datas/meshes/Ghost/BarreV.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	BarreV[0]->SetTransp( MatTranspAddAlpha )															;
	BarreV[0]->InterpoAlpha( 0.0f, 1.0f, TAPPEAR )														;
	BarreV[0]->Disable()																				;

	for( U32 a=0; a<3; a++)
	{
		BarreV[a+1] = new UImage	;
		*BarreV[a+1] = *BarreV[0]	;
	}


	BarreH[0] = new UImage( "Datas/meshes/Ghost/BarreH.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	BarreH[0]->SetTransp( MatTranspAddAlpha )															;
	BarreH[0]->InterpoAlpha( 0.0f, 1.0f, TAPPEAR )														;
	BarreH[0]->Disable()																				;

	for( U32 a=0; a<2; a++)
	{
		BarreH[a+1] = new UImage	;
		*BarreH[a+1] = *BarreH[0]	;
	}

	disamode = FALSE	;
	time = -1000.0f		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
Ghost2D::~Ghost2D()
{
	for( U32 a=0; a<3; a++)
		delete Rond[a]	;

	for( U32 a=0; a<4; a++)
		delete BarreV[a]	;

	for( U32 a=0; a<3; a++)
		delete BarreH[a]	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void Ghost2D::Go()
{
	Rond[0]->Enable()										;
	Rond[0]->SetPos( GetUniXpos(68.f), GetUniYpos(414.f) )	;
	Rond[0]->InterpoZoom( .2f, 1.2f, DUREE )				;

	Rond[1]->Enable()										;
	Rond[1]->SetPos( GetUniXpos(80.f), GetUniYpos(402.f) )	;
	Rond[1]->InterpoZoom( 0.8f, 0.36f, DUREE )				;

	Rond[2]->Enable()										;
	Rond[2]->SetPos( GetUniXpos(87.f), GetUniYpos(380.f) )	;
	Rond[2]->InterpoZoom( 0.5f, 2.00f, DUREE )				;

	BarreV[0]->Enable()																								;
	BarreV[0]->SetPos( GetUniXpos(25.f), GetUniYpos(272.f) )														;
	BarreV[0]->InterpoYPos( BarreV[0]->GetYpos()-GetUniYpos(20.f), BarreV[0]->GetYpos()+GetUniYpos(120.f), DUREE )	;

	BarreV[1]->Enable()																								;
	BarreV[1]->SetPos( GetUniXpos(42.f), GetUniYpos(218.f) )														;
	BarreV[1]->SetYSize( .5f )																						;
	BarreV[1]->InterpoYPos( BarreV[1]->GetYpos()-GetUniYpos(50.f), BarreV[1]->GetYpos()+GetUniYpos(60.f), DUREE )	;

	BarreV[2]->Enable()																								;
	BarreV[2]->SetPos( GetUniXpos(58.f), GetUniYpos(243.f) )														;
	BarreV[2]->SetYSize( .76f )																						;
	BarreV[2]->InterpoYPos( BarreV[2]->GetYpos()+GetUniYpos(20.f), BarreV[2]->GetYpos()-GetUniYpos(50.f), DUREE )	;

	BarreV[3]->Enable()																								;
	BarreV[3]->SetPos( GetUniXpos(73.f), GetUniYpos(172.f) )														;
	BarreV[3]->SetYSize( .6f )																						;
	BarreV[3]->InterpoYPos( BarreV[3]->GetYpos()-GetUniYpos(60.f), BarreV[3]->GetYpos()+GetUniYpos(130.f), DUREE )	;

	BarreH[0]->Enable()																								;
	BarreH[0]->SetPos( GetUniXpos(200.f), GetUniYpos(417.f) )														;
	BarreH[0]->SetXSize( .6f )																						;
	BarreH[0]->InterpoXPos( BarreH[0]->GetXpos()-GetUniXpos(60.f), BarreH[0]->GetXpos()+GetUniXpos(160.f), DUREE )	;

	BarreH[1]->Enable()																								;
	BarreH[1]->SetPos( GetUniXpos(370.f), GetUniYpos(431.f) )														;
	BarreH[1]->SetXSize( .66f )																						;
	BarreH[1]->InterpoXPos( BarreH[0]->GetXpos()+GetUniXpos(40.f), BarreH[0]->GetXpos()-GetUniXpos(150.f), DUREE )	;

	BarreH[2]->Enable()																								;
	BarreH[2]->SetPos( GetUniXpos(50.f), GetUniYpos(445.f) )														;
	BarreH[2]->SetXSize( .33f )																						;
	BarreH[2]->InterpoXPos( BarreH[2]->GetXpos()-GetUniXpos(40.f), BarreH[2]->GetXpos()+GetUniXpos(180.f), DUREE )	;

	time = 0.0f			;
	disamode = FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void Ghost2D::DisappearAll()
{
	for( U32 a=0; a<3; a++ )
		Rond[a]->InterpoAlpha( 1.0f, 0.0f, TDISAPPEAR )	;

	for( U32 a=0; a<4; a++ )
		BarreV[a]->InterpoAlpha( 1.0f, 0.0f, TDISAPPEAR )	;

	for( U32 a=0; a<3; a++ )
		BarreH[a]->InterpoAlpha( 1.0f, 0.0f, TDISAPPEAR )	;
	
	disamode = TRUE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void Ghost2D::UpdateFrame( Ufloat laptime )
{
	time += laptime		;
	if( time >= TEMPS_NORM+TAPPEAR )
		if( !disamode )
			DisappearAll()	;

	for( U32 a=0; a<3; a++ )
	{
		Rond[a]->Affiche( laptime )	;
		if( disamode )
			if( !Rond[a]->IsInterpoAlpha() )
				Rond[a]->Disable()	;
	}

	for( U32 a=0; a<4; a++ )
	{
		BarreV[a]->Affiche( laptime )	;
		if( disamode )
			if( !BarreV[a]->IsInterpoAlpha() )
				BarreV[a]->Disable()	;
	}

	for( U32 a=0; a<3; a++ )
	{
		BarreH[a]->Affiche( laptime )	;
		if( disamode )
			if( !BarreH[a]->IsInterpoAlpha() )
				BarreH[a]->Disable()	;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
