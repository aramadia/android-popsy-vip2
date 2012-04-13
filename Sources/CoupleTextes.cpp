					 /*---------------------------------------------------------------------+
					  |						CoupleTextes.cpp								|
					  |																		|
					  |	affichera des textes remplient d'une puissance philosophik encore	|
					  |						méconnue par l'homme .							|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class CoupleTextes	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CoupleTextes::CoupleTextes()
{
	Sub	=new UImage( "Datas/meshes/Couple/sub.jpg", BILINEAR_TEXTURE, 512.f, 32.f )	;
	Sub->SetPos( 0.5f, 0.35f )															;
	Sub->SetTransp( MatTranspAddAlpha )													;
	Sub->SetZoom( 3.0f )																;

	Nihil = new UImage( "Datas/meshes/Couple/Nihil.jpg", BILINEAR_TEXTURE, 512.f, 32.f )	;
	Nihil->SetPos( 0.5f, 0.6f )																;
	Nihil->SetTransp( MatTranspAddAlpha )													;
	Nihil->SetZoom( 3.0f )																	;

	Corres = new UImage( "Datas/meshes/Couple/corres.jpg", BILINEAR_TEXTURE, 32.f, 512.f  )	;
	Corres->SetTransp( MatTranspAddAlpha )														;
	Corres->SetPos( .25f, .5f )																	;
	Corres->Disable()																			;

	Revive = new UImage	( "Datas/meshes/Couple/revive.jpg", BILINEAR_TEXTURE, 64.f, 512.f )	;
	Revive->SetPos( Revive->GetXSize()*0.5f, Revive->GetYSize()*0.5f )							;
	Revive->SetTransp( MatTranspAddAlpha )														;
	Revive->InterpoYPos( Revive->GetYSize()*0.7f, 0.0f, 15.f )									;

	Revive1	 = new UImage												;
	(*Revive1) = (*Revive)												;
	Revive1->SetZoom( 1.5f )											;
	Revive1->SetAlpha( .3f )											;
	Revive1->SetPos( Revive1->GetXSize()*.6f, Revive->GetYSize()*0.6f )	;
	Revive1->InterpoYPos( Revive1->GetYSize()*0.7f, -0.1f, 15.f )		;

	Again = new UImage	( "Datas/meshes/Couple/Again.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;
	Again->SetPos( .5f, .5f )																		;
	Again->SetTransp( MatTranspAddAlpha )															;
	Again->Disable()																				;
	timeAgain = totaltimeAgain = 0.0f																;
	DisaAgain = FALSE																				;
}
//----------------------------------------------------------------------------------------------------------------------------------------
CoupleTextes::~CoupleTextes()
{
	delete Sub		;
	delete Nihil	;
	delete Corres	;
	delete Revive	;
	delete Revive1	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleTextes::GereXImage( UImage* ima, Ufloat laptime, bool right  )
{
	if( !ima->IsInterpoXPos() )
	{
		if( right )
		{
			ima->SetPos( 1.0f+ima->GetXSize()*.5f, .35f )									;
			ima->InterpoXPos( ima->GetXpos(), -ima->GetXSize()*.5f, 4.0f + 3.f*Random1() )	;
		}
		else
		{
			ima->SetPos( -ima->GetXSize()*.5f, .65f )												;
			ima->InterpoXPos( ima->GetXpos(), ima->GetXSize()*.5f + 1.0f, 4.0f + 3.f*Random1() )	;
		}
	}

	ima->Affiche( laptime )	;	
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleTextes::UpdateFrameBack( Ufloat time )
{
	GereXImage( Sub, time, TRUE  )		;
	GereXImage( Nihil, time, FALSE  )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleTextes::GoCorres()
{
	Corres->InterpoYPos( Corres->GetYSize()*.5f + 1.0f, -Corres->GetYSize()*.5f, 5.f )	;
	Corres->Enable()																	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleTextes::GoAgain( Ufloat time )
{
	Again->Enable()								;
	Again->InterpoYPos( .6f, .5f, time*.5f )	;
	Again->InterpoAlpha( 0.0f, 1.0f, time*.5f )	;
	Again->InterpoZoom( .5f, 3.f, time )		;
	totaltimeAgain = time						;
	timeAgain = time*.5f						;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleTextes::UpdateFrameFore( Ufloat time )
{
	Corres->Affiche( time )	;
	if( !Corres->IsInterpoYPos() )
		Corres->Disable()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleTextes::UpdateFRameFore1( Ufloat laptime )
{
	Revive->Affiche( laptime )	;
	Revive1->Affiche( laptime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleTextes::UpdateFrameForeAll( Ufloat laptime )
{
	Again->Affiche( laptime )	;
	if( Again->IsEnable() )
	{
		timeAgain -= laptime	;
		if( (timeAgain < 0.0f) && !DisaAgain)
		{
			Again->InterpoAlpha( 1.0f, 0.0f, totaltimeAgain*.5f )	;
			DisaAgain = TRUE										;
		}
		else
		{
			if( !Again->IsInterpoAlpha() )
				Again->Disable()	;
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
