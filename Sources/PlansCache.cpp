	   				 /*---------------------------------------------------------------------+
					  |							PlansCache.cpp								|
					  |																		|
					  |			une jolie partie cache ki regorge fiéfés faciesses			|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/


#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|  Class PlansCache	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
PlansCache::PlansCache( Ufloat time )
{
	Tuo = new UImage( "Datas/meshes/Cache1/Tuo.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Tuo->OpenMask( "Datas/meshes/Cache1/mask.raw" )										;
	Tuo->SetUpLeftPos( 0.0f, GetUniYpos( 220.f)	)											;
	Tuo->SetTransp( MatTranspAlpha )														;
	Tuo->Disable()																			;
	BTuo = FALSE	;

	U2 = new UImage( "Datas/meshes/Cache1/U2.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	U2->OpenMask( "Datas/meshes/Cache1/mask.raw" )									;
	U2->SetUpLeftPos( 0.0f, GetUniYpos( 220.f)	)										;
	U2->SetTransp( MatTranspAlpha )														;
	U2->Disable()																		;
	BU2 = FALSE	;

	Ghell = new UImage( "Datas/meshes/Cache1/Ghell.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Ghell->OpenMask( "Datas/meshes/Cache1/mask.raw" )										;
	Ghell->SetUpLeftPos( 0.0f, GetUniYpos( 220.f)	)											;
	Ghell->SetTransp( MatTranspAlpha )															;
	Ghell->Disable()																			;
	BGhell = FALSE	;

	Madman = new UImage( "Datas/meshes/Cache1/Madman.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Madman->OpenMask( "Datas/meshes/Cache1/mask.raw" )										;
	Madman->SetUpLeftPos( 0.0f, GetUniYpos( 220.f)	)											;
	Madman->SetTransp( MatTranspAlpha )															;
	Madman->Disable()																			;
	BMadman = FALSE	;

	Taggy = new UImage( "Datas/meshes/Cache1/Taggy.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Taggy->OpenMask( "Datas/meshes/Cache1/mask.raw" )										;
	Taggy->SetUpLeftPos( 0.0f, GetUniYpos( 220.f)	)											;
	Taggy->SetTransp( MatTranspAlpha )															;
	Taggy->Disable()																			;
	BTaggy = FALSE	;

	FFactory = new UImage( "Datas/meshes/Cache1/FF.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	FFactory->OpenMask( "Datas/meshes/Cache1/mask.raw" )										;
	FFactory->SetUpLeftPos( 0.0f, GetUniYpos( 220.f)	)										;
	FFactory->SetTransp( MatTranspAlpha )														;
	FFactory->Disable()																			;
	BFFactory = FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
PlansCache::~PlansCache()
{
	delete Tuo		;
	delete U2		;
	delete Ghell	;
	delete Madman	;
	delete Taggy	;
	delete FFactory	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnAppearTuo( Ufloat time )
{
	Tuo->InterpoAlpha( 0.0f, 1.0f, time )											;
	Tuo->InterpoXPos( Tuo->GetXpos() - GetUniXpos( 50.f), Tuo->GetXpos(), time )	;
	Tuo->Enable()																	;
	BTuo = FALSE																	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnDisappearTuo( Ufloat time )
{
	Tuo->InterpoAlpha( 1.0f, 0.0f, time )											;
	Tuo->InterpoXPos( Tuo->GetXpos(), Tuo->GetXpos() + GetUniXpos( 100.f), time )	;
	BTuo = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnAppearU2( Ufloat time )
{
	U2->InterpoAlpha( 0.0f, 1.0f, time )										;
	U2->InterpoXPos( U2->GetXpos() - GetUniXpos( 50.f), U2->GetXpos(), time )	;
	U2->Enable()																;
	BU2 = FALSE																	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnDisappearU2( Ufloat time )
{
	U2->InterpoAlpha( 1.0f, 0.0f, time )										;
	U2->InterpoXPos( U2->GetXpos(), U2->GetXpos() + GetUniXpos( 100.f), time )	;
	BU2 = TRUE																	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnAppearMadman( Ufloat time )
{
	Madman->InterpoAlpha( 0.0f, 1.0f, time )												;
	Madman->InterpoXPos( Madman->GetXpos() - GetUniXpos( 50.f), Madman->GetXpos(), time )	;
	Madman->Enable()																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnDisappearMadman( Ufloat time )
{
	Madman->InterpoAlpha( 1.0f, 0.0f, time )												;
	Madman->InterpoXPos( Madman->GetXpos(), Madman->GetXpos() + GetUniXpos( 100.f), time )	;
	BMadman = TRUE																			;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnAppearGhell( Ufloat time )
{
	Ghell->InterpoAlpha( 0.0f, 1.0f, time )												;
	Ghell->InterpoXPos( Ghell->GetXpos() - GetUniXpos( 50.f), Ghell->GetXpos(), time )	;
	Ghell->Enable()																		;
	BGhell = FALSE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnDisappearGhell( Ufloat time )
{
	Ghell->InterpoAlpha( 1.0f, 0.0f, time )												;
	Ghell->InterpoXPos( Ghell->GetXpos(), Ghell->GetXpos() + GetUniXpos( 100.f), time )	;
	BGhell = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnAppearTaggy( Ufloat time )
{
	Taggy->InterpoAlpha( 0.0f, 1.0f, time )												;
	Taggy->InterpoXPos( Taggy->GetXpos() - GetUniXpos( 50.f), Taggy->GetXpos(), time )	;
	Taggy->Enable()																		;
	BTaggy = FALSE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnDisappearTaggy( Ufloat time )
{
	Taggy->InterpoAlpha( 1.0f, 0.0f, time )												;
	Taggy->InterpoXPos( Taggy->GetXpos(), Taggy->GetXpos() + GetUniXpos( 100.f), time )	;
	BTaggy = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnAppearFFactory( Ufloat time )
{
	FFactory->InterpoAlpha( 0.0f, 1.0f, time )													;
	FFactory->InterpoXPos( FFactory->GetXpos() - GetUniXpos( 50.f), FFactory->GetXpos(), time )	;
	FFactory->Enable()																			;
	BFFactory = FALSE																			;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::OnDisappearFFactory( Ufloat time )
{
	FFactory->InterpoAlpha( 1.0f, 0.0f, time )												;
	FFactory->InterpoXPos( Taggy->GetXpos(), Taggy->GetXpos() + GetUniXpos( 100.f), time )	;
	BFFactory = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansCache::UpdateFrame( Ufloat laptime )
{
	U2->Affiche( laptime )			;
		if( BU2 && !U2->IsInterpoAlpha() )
			U2->Disable()	;

	Tuo->Affiche( laptime )			;
		if( BTuo && !Tuo->IsInterpoAlpha() )
			Tuo->Disable()	;

	Ghell->Affiche( laptime )			;
		if( BGhell && !Ghell->IsInterpoAlpha() )
			Ghell->Disable()	;

	Madman->Affiche( laptime )			;
		if( BMadman && !Madman->IsInterpoAlpha() )
			Madman->Disable()	;

	Taggy->Affiche( laptime )			;
		if( BTaggy && !Taggy->IsInterpoAlpha() )
			Taggy->Disable()	;

	FFactory->Affiche( laptime )			;
		if( BFFactory && !FFactory->IsInterpoAlpha() )
			FFactory->Disable()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------


