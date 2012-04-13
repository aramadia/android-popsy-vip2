	   				 /*---------------------------------------------------------------------+
					  |							PlansTextes.cpp								|
					  |																		|
					  |			affichage de textes dans la partie des pllans voiliii		|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class PlansTextes	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
PlansTextes::PlansTextes( Ufloat time )
{
	DStaff = new UImage( "Datas/meshes/Plans/dstaff.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	DStaff->OpenMask( "Datas/meshes/Plans/dstaff.raw" )										;
	DStaff->SetUpLeftPos( GetUniXpos( 370.f), GetUniYpos( 20.f)	)								;
	DStaff->SetTransp( MatTranspAlpha )															;
	DStaff->InterpoAlpha( 0.0f, 1.0f, time )													;
	DStaff->InterpoXPos( GetUniXpos( 270.f), GetUniXpos( 370.f), time )							;
	BDStaff = FALSE	;

	Programing = new UImage( "Datas/meshes/Plans/Programing.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Programing->OpenMask( "Datas/meshes/Plans/Programing.raw" )										;
	Programing->SetUpLeftPos( GetUniXpos( 354.f), GetUniYpos( 332.f)	)								;
	Programing->SetTransp( MatTranspAlpha )																;
	Programing->Disable()																				;
	BPrograming = FALSE	;	

	Gfx2d = new UImage( "Datas/meshes/Plans/2dgfx.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Gfx2d->OpenMask( "Datas/meshes/Plans/2dgfx.raw" )									;
	Gfx2d->SetUpLeftPos( GetUniXpos( 354.f), GetUniYpos( 332.f)	)							;
	Gfx2d->SetTransp( MatTranspAlpha )														;
	Gfx2d->Disable()																		;
	BGfx2d = FALSE	;

	Gfx3d = new UImage( "Datas/meshes/Plans/3dgfx.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Gfx3d->OpenMask( "Datas/meshes/Plans/3dgfx.raw" )									;
	Gfx3d->SetTransp( MatTranspAlpha )														;
	Gfx3d->Disable()																		;
	BGfx3d = FALSE	;

	Music = new UImage( "Datas/meshes/Plans/music.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Music->OpenMask( "Datas/meshes/Plans/music.raw" )									;
	Music->SetTransp( MatTranspAlpha )														;
	Music->Disable()																		;
	BMusic = FALSE	;

	Tuo = new UImage( "Datas/meshes/Plans/Tuo.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Tuo->OpenMask( "Datas/meshes/Plans/Tuo.raw" )									;
	Tuo->SetUpLeftPos( GetUniXpos( 408.f), GetUniYpos( 374.f)	)						;
	Tuo->SetTransp( MatTranspAlpha )													;
	Tuo->Disable()																		;
	BTuo = FALSE	;

	U2 = new UImage( "Datas/meshes/Plans/U2.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	U2->OpenMask( "Datas/meshes/Plans/U2.raw" )										;
	U2->SetUpLeftPos( GetUniXpos( 423.f), GetUniYpos( 377.f)	)						;
	U2->SetTransp( MatTranspAlpha )														;
	U2->Disable()																		;
	BU2 = FALSE	;

	Ghell = new UImage( "Datas/meshes/Plans/Ghell.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Ghell->OpenMask( "Datas/meshes/Plans/Ghell.raw" )									;
	Ghell->SetUpLeftPos( GetUniXpos( 354.f), GetUniYpos( 377.f)	)							;
	Ghell->SetTransp( MatTranspAlpha )														;
	Ghell->Disable()																		;
	BGhell = FALSE	;

	Madman = new UImage( "Datas/meshes/Plans/Madman.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Madman->OpenMask( "Datas/meshes/Plans/Madman.raw" )										;
	Madman->SetUpLeftPos( GetUniXpos( 354.f), GetUniYpos( 377.f)	)							;
	Madman->SetTransp( MatTranspAlpha )															;
	Madman->Disable()																			;
	BMadman = FALSE	;

	Taggy = new UImage( "Datas/meshes/Plans/Taggy.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Taggy->OpenMask( "Datas/meshes/Plans/Taggy.raw" )									;
	Taggy->SetUpLeftPos( GetUniXpos( 354.f), GetUniYpos( 377.f)	)							;
	Taggy->SetTransp( MatTranspAlpha )														;
	Taggy->Disable()																		;
	BTaggy = FALSE	;

	FFactory = new UImage( "Datas/meshes/Plans/FF.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	FFactory->OpenMask( "Datas/meshes/Plans/FF.raw" )									;
	FFactory->SetUpLeftPos( GetUniXpos( 354.f), GetUniYpos( 377.f)	)						;
	FFactory->SetTransp( MatTranspAlpha )													;
	FFactory->Disable()																		;
	BFFactory = FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
PlansTextes::~PlansTextes()
{
	delete DStaff		;
	delete Programing	;
	delete Gfx2d		;
	delete Gfx3d		;
	delete Music		;
	delete Tuo			;
	delete U2			;
	delete Ghell		;
	delete Madman		;
	delete Taggy		;
	delete FFactory		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappear( Ufloat time )
{
	DStaff->InterpoAlpha( 1.0f, 0.0f, time )												;
	DStaff->InterpoXPos( DStaff->GetXpos(), DStaff->GetXpos() + GetUniXpos( 100.f), time )	;
	BPrograming = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearPrograming( Ufloat time )
{
	Programing->InterpoAlpha( 0.0f, 1.0f, time )														;
	Programing->InterpoXPos( Programing->GetXpos() + GetUniXpos( 100.f), Programing->GetXpos(), time )	;
	Programing->Enable()																				;
	BPrograming = FALSE																					;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearPrograming( Ufloat time )
{
	Programing->InterpoAlpha( 1.0f, 0.0f, time )														;
	Programing->InterpoXPos( Programing->GetXpos(), Programing->GetXpos() - GetUniXpos( 100.f), time )	;
	BPrograming = TRUE																					;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearGfx2d( Ufloat time )
{
	Gfx2d->InterpoAlpha( 0.0f, 1.0f, time )												;
	Gfx2d->InterpoXPos( Gfx2d->GetXpos() + GetUniXpos( 100.f), Gfx2d->GetXpos(), time )	;
	Gfx2d->Enable()																		;
	BGfx2d = FALSE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearGfx2d( Ufloat time )
{
	Gfx2d->InterpoAlpha( 1.0f, 0.0f, time )												;
	Gfx2d->InterpoXPos( Gfx2d->GetXpos(), Gfx2d->GetXpos() - GetUniXpos( 100.f), time )	;
	BGfx2d = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearGfx3d( Ufloat time )
{
	Gfx3d->InterpoAlpha( 0.0f, 1.0f, time )												;
	Gfx3d->SetUpLeftPos( GetUniXpos( 354.f), GetUniYpos( 332.f)	)						;
	Gfx3d->InterpoXPos( Gfx3d->GetXpos() + GetUniXpos( 100.f), Gfx3d->GetXpos(), time )	;
	Gfx3d->Enable()																		;
	BGfx3d = FALSE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearGfx3d( Ufloat time )
{
	Gfx3d->InterpoAlpha( 1.0f, 0.0f, time )												;
	Gfx3d->InterpoXPos( Gfx3d->GetXpos(), Gfx3d->GetXpos() - GetUniXpos( 100.f), time )	;
	BGfx3d = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearMusic( Ufloat time )
{
	Music->InterpoAlpha( 0.0f, 1.0f, time )												;
	Music->SetUpLeftPos( GetUniXpos( 354.f), GetUniYpos( 332.f)	)						;
	Music->InterpoXPos( Music->GetXpos() + GetUniXpos( 100.f), Music->GetXpos(), time )	;
	Music->Enable()																		;
	BMusic = FALSE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearMusic( Ufloat time )
{
	Music->InterpoAlpha( 1.0f, 0.0f, time )												;
	Music->InterpoXPos( Music->GetXpos(), Music->GetXpos() - GetUniXpos( 100.f), time )	;
	BMusic = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearTuo( Ufloat time )
{
	Tuo->InterpoAlpha( 0.0f, 1.0f, time )											;
	Tuo->InterpoXPos( Tuo->GetXpos() - GetUniXpos( 50.f), Tuo->GetXpos(), time )	;
	Tuo->Enable()																	;
	BTuo = FALSE																	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearTuo( Ufloat time )
{
	Tuo->InterpoAlpha( 1.0f, 0.0f, time )											;
	Tuo->InterpoXPos( Tuo->GetXpos(), Tuo->GetXpos() + GetUniXpos( 100.f), time )	;
	BTuo = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearU2( Ufloat time )
{
	U2->InterpoAlpha( 0.0f, 1.0f, time )										;
	U2->InterpoXPos( U2->GetXpos() - GetUniXpos( 50.f), U2->GetXpos(), time )	;
	U2->Enable()																;
	BU2 = FALSE																	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearU2( Ufloat time )
{
	U2->InterpoAlpha( 1.0f, 0.0f, time )										;
	U2->InterpoXPos( U2->GetXpos(), U2->GetXpos() + GetUniXpos( 100.f), time )	;
	BU2 = TRUE																	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearMadman( Ufloat time )
{
	Madman->InterpoAlpha( 0.0f, 1.0f, time )												;
	Madman->InterpoXPos( Madman->GetXpos() - GetUniXpos( 50.f), Madman->GetXpos(), time )	;
	Madman->Enable()																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearMadman( Ufloat time )
{
	Madman->InterpoAlpha( 1.0f, 0.0f, time )												;
	Madman->InterpoXPos( Madman->GetXpos(), Madman->GetXpos() + GetUniXpos( 100.f), time )	;
	BMadman = TRUE																			;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearGhell( Ufloat time )
{
	Ghell->InterpoAlpha( 0.0f, 1.0f, time )												;
	Ghell->InterpoXPos( Ghell->GetXpos() - GetUniXpos( 50.f), Ghell->GetXpos(), time )	;
	Ghell->Enable()																		;
	BGhell = FALSE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearGhell( Ufloat time )
{
	Ghell->InterpoAlpha( 1.0f, 0.0f, time )												;
	Ghell->InterpoXPos( Ghell->GetXpos(), Ghell->GetXpos() + GetUniXpos( 100.f), time )	;
	BGhell = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearTaggy( Ufloat time )
{
	Taggy->InterpoAlpha( 0.0f, 1.0f, time )												;
	Taggy->InterpoXPos( Taggy->GetXpos() - GetUniXpos( 50.f), Taggy->GetXpos(), time )	;
	Taggy->Enable()																		;
	BTaggy = FALSE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearTaggy( Ufloat time )
{
	Taggy->InterpoAlpha( 1.0f, 0.0f, time )												;
	Taggy->InterpoXPos( Taggy->GetXpos(), Taggy->GetXpos() + GetUniXpos( 100.f), time )	;
	BTaggy = TRUE																		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnAppearFFactory( Ufloat time )
{
	FFactory->InterpoAlpha( 0.0f, 1.0f, time )													;
	FFactory->InterpoXPos( FFactory->GetXpos() - GetUniXpos( 50.f), FFactory->GetXpos(), time )	;
	FFactory->Enable()																			;
	BFFactory = FALSE																			;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::OnDisappearFFactory( Ufloat time )
{
	FFactory->InterpoAlpha( 1.0f, 0.0f, time )														;
	FFactory->InterpoXPos( FFactory->GetXpos(), FFactory->GetXpos() + GetUniXpos( 100.f), time )	;
	BFFactory = TRUE																				;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTextes::UpdateFrame( Ufloat laptime )	
{
	DStaff->Affiche( laptime )		;
		if( BDStaff &&  !DStaff->IsInterpoAlpha() )
			DStaff->Disable()	;

	Programing->Affiche( laptime )	;
		if( BPrograming && !Programing->IsInterpoAlpha() )
			Programing->Disable()	;

	Gfx2d->Affiche( laptime )	;
		if( BGfx2d && !Gfx2d->IsInterpoAlpha() )
			Gfx2d->Disable()	;

	Gfx3d->Affiche( laptime )	;
		if( BGfx3d && !Gfx3d->IsInterpoAlpha() )
			Gfx3d->Disable()	;

	Music->Affiche( laptime )	;
		if( BMusic && !Music->IsInterpoAlpha() )
			Music->Disable()	;

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

	FFactory->Affiche( laptime )	;
		if( BFFactory && !FFactory->IsInterpoAlpha() )
			FFactory->Disable()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
