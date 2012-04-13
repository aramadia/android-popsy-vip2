    					 /*-----------------------------------------------------+
						  |				       NebuTexte.cpp					|
						  |														|
						  |	Affichage des giga paroles du chanteur de la chanson|
						  |														|
						  | 	U2^PoPsy TeAm 2000								|
						  +-----------------------------------------------------*/

#include "stdafx.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|    Class NebuTxt	|
//										+-------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
NebuTxt::NebuTxt()
{
	TextureManager::SetRepertoire( "" )	;

	texture1 = TextureManager::OpenJpg( "Datas/meshes/nebuleuse/nebu lettres.jpg", BILINEAR_TEXTURE )	;
	texture2 = TextureManager::OpenJpg( "Datas/meshes/nebuleuse/nebu lettres1.jpg", BILINEAR_TEXTURE )	;
	texture3 = TextureManager::OpenJpg( "Datas/meshes/nebuleuse/nebu lettres2.jpg", BILINEAR_TEXTURE )	;

	Forsaken1 = new UImage							;
	Forsaken1->SetTexture( texture1, 512.f, 128.f )	;
	Forsaken1->SetCoordRV( 0.f, 325.f, 0.0f, 68.f )	;	// coordonnées a l'intérieur de la texture
	Forsaken1->SetTransp( MatTranspAddAlpha )		;
	Forsaken1->Disable()							;

	Forsaken2 = new UImage	;
	*Forsaken2 = *Forsaken1	;


	by1 = new UImage							;
	by1->SetTexture( texture3, 512.f, 128.f  )	;
	by1->SetCoordRV( 48.f, 156.f, 12.0f, 118.f );
	by1->SetTransp( MatTranspAddAlpha )			;
	by1->Disable()								;

	by2 = new UImage	;
	*by2 = *by1			;

	destiny = new UImage							;
	destiny->SetTexture( texture1, 512.f, 128.f  )	;
	destiny->SetCoordRV( 0.f, 204.0f, 67.f, 127.f )	;
	destiny->SetTransp( MatTranspAddAlpha )			;
	destiny->Disable()								;

	my1 = new UImage								;
	my1->SetTexture( texture3, 512.f, 128.f  )		;
	my1->SetCoordRV( 155.f, 280.f, 28.0f, 155.f )	;
	my1->SetTransp( MatTranspAddAlpha )				;
	my1->Disable()									;

	my2 = new UImage	;
	*my2 = *my1			;

	own = new UImage							;
	own->SetTexture( texture1, 512.f, 128.f  )	;
	own->SetCoordRV( 325.f, 490.f, 5.0f, 68.f )	;
	own->SetTransp( MatTranspAddAlpha )			;
	own->Disable()								;

	mind = new UImage							;
	mind->SetTexture( texture2, 512.f, 128.f )	;
	mind->SetCoordRV( 0.f, 185.f, 0.0f, 70.f )	;
	mind->SetTransp( MatTranspAddAlpha )		;
	mind->Disable()								;

	I = new UImage							;
	I->SetTexture( texture3, 512.f, 128.f )	;
	I->SetCoordRV( 8.f, 49.f, 9.0f, 112.f )	;
	I->SetTransp( MatTranspAddAlpha )		;
	I->Disable()							;

	must = new UImage								;
	must->SetTexture( texture3, 512.f, 128.f )		;
	must->SetCoordRV( 280.f, 506.f, 20.0f, 108.f )	;
	must->SetTransp( MatTranspAddAlpha )			;
	must->Disable()									;

	remove = new UImage									;
	remove->SetTexture( texture1, 512.f, 128.f )		;
	remove->SetCoordRV( 211.f, 402.f, 73.0f, 123.f )	;
	remove->SetTransp( MatTranspAddAlpha )				;
	remove->Disable()									;

	skin = new UImage								;
	skin->SetTexture( texture2, 512.f, 128.f )		;
	skin->SetCoordRV( 158.f, 276.f, 76.0f, 127.f )	;
	skin->SetTransp( MatTranspAddAlpha )			;
	skin->Disable()									;

	to = new UImage									;
	to->SetTexture( texture2, 512.f, 128.f )		;
	to->SetCoordRV( 414.f, 483.f, 66.0f, 120.f )	;
	to->SetTransp( MatTranspAddAlpha )				;
	to->Disable()									;

	see = new UImage							;
	see->SetTexture( texture2, 512.f, 128.f )	;
	see->SetCoordRV( 367.f, 497.f, 2.0f, 62.f )	;
	see->SetTransp( MatTranspAddAlpha )			;
	see->Disable()								;

	belief = new UImage								;
	belief->SetTexture( texture2, 512.f, 128.f )	;
	belief->SetCoordRV( 4.f, 156.f, 69.0f, 124.f )	;
	belief->SetTransp( MatTranspAddAlpha )			;
	belief->Disable()								;

	in = new UImage									;
	in->SetTexture( texture1, 512.f, 128.f )		;
	in->SetCoordRV( 411.f, 482.f, 66.0f, 122.f )	;
	in->SetTransp( MatTranspAddAlpha )				;
	in->Disable()									;

	your = new UImage								;
	your->SetTexture( texture2, 512.f, 128.f )		;
	your->SetCoordRV( 276.f, 407.f, 68.0f, 126.f )	;
	your->SetTransp( MatTranspAddAlpha )			;
	your->Disable()									;

	eyes = new UImage								;
	eyes->SetTexture( texture2, 512.f, 128.f )		;
	eyes->SetCoordRV( 190.f, 364.f, 0.0f, 71.f )	;
	eyes->SetTransp( MatTranspAddAlpha )			;
	eyes->Disable()									;
}
//----------------------------------------------------------------------------------------------------------
NebuTxt::~NebuTxt()
{
	delete Forsaken1	;
	delete Forsaken2	;
	delete by1			;
	delete by2			;
	delete destiny		;
	delete my1			;
	delete my2			;
	delete own			;
	delete mind			;
	delete I			;
	delete must			;
	delete remove		;
	delete skin			;
	delete to			;
	delete see			;
	delete belief		;
	delete in			;
	delete your			;
	delete eyes			;

	TextureManager::SubUse( texture1 )	;
	TextureManager::SubUse( texture2 )	;
	TextureManager::SubUse( texture3 )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnForsaken1()
{
	Forsaken1->Enable()																					;
	Forsaken1->SetUpLeftPos( GetUniXpos( 3.f-40.f ), GetUniYpos( 70.f ) )								;
	Forsaken1->InterpoAlpha( 0.0f, 1.0f, 2.0f )															;
	Forsaken1->InterpoXPos( Forsaken1->GetXpos(), Forsaken1->GetXpos() + GetUniXpos( 180.f ), 12.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnBy1()
{
	by1->Enable()																	;
	by1->SetUpLeftPos( GetUniXpos( 317.f ), GetUniYpos( 54.f-40.f ) )				;
	by1->InterpoAlpha( 0.0f, 1.0f, 2.0f )											;
	by1->InterpoYPos( by1->GetYpos(), by1->GetYpos() + GetUniYpos( 160.f ), 11.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnDestiny()
{
	destiny->Enable()																				;
	destiny->SetUpLeftPos( GetUniXpos( 400.f+40.f ), GetUniYpos( 116.f ) )							;
	destiny->InterpoAlpha( 0.0f, 1.0f, 2.0f )														;
	destiny->InterpoXPos(  destiny->GetXpos(), destiny->GetXpos() - GetUniXpos( 150.f ), 10.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnForsaken2()
{
	Forsaken2->Enable()																				;
	Forsaken2->SetUpLeftPos( GetUniXpos( 4.f ), GetUniYpos( 195.f+40.f ) )							;
	Forsaken2->InterpoAlpha( 0.0f, 1.0f, 2.0f )														;
	Forsaken2->InterpoYPos( Forsaken2->GetYpos(), Forsaken2->GetYpos() - GetUniYpos( 80.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnBy2()
{
	by2->Enable()																	;
	by2->SetUpLeftPos( GetUniXpos( 38.f+40.f ), GetUniYpos( 256.f ) )				;
	by2->InterpoAlpha( 0.0f, 1.0f, 2.0f )											;
	by2->InterpoXPos( by2->GetXpos(), by2->GetXpos() - GetUniXpos( 30.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnMy1()
{
	my1->Enable()																	;
	my1->SetUpLeftPos( GetUniXpos( 141.f-20.f ), GetUniYpos( 256.f ) )				;
	my1->InterpoAlpha( 0.0f, 1.0f, 2.0f )											;
	my1->InterpoXPos( my1->GetXpos(), my1->GetXpos() + GetUniXpos( 60.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnOwn()
{
	own->Enable()																	;
	own->SetUpLeftPos( GetUniXpos( 2.f ), GetUniYpos( 325.f-10.f ) )				;
	own->InterpoAlpha( 0.0f, 1.0f, 2.0f )											;
	own->InterpoYPos( own->GetYpos(), own->GetYpos() + GetUniYpos( 60.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnMind()
{
	mind->Enable()																		;
	mind->SetUpLeftPos( GetUniXpos( 148.f+40.f ), GetUniYpos( 335.f ) )					;
	mind->InterpoAlpha( 0.0f, 1.0f, 2.0f )												;
	mind->InterpoXPos( mind->GetXpos(), mind->GetXpos() - GetUniXpos( 100.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnI()
{
	I->Enable()																;
	I->SetUpLeftPos( GetUniXpos( 152.f ), GetUniYpos( 1.f-10.f ) )			;
	I->InterpoAlpha( 0.0f, 1.0f, 2.0f )										;
	I->InterpoYPos( I->GetYpos(), I->GetYpos() + GetUniYpos( 40.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnMust()
{
	must->Enable()																		;
	must->SetUpLeftPos( GetUniXpos( 200.f+20.f ), GetUniYpos( 7.f ) )					;
	must->InterpoAlpha( 0.0f, 1.0f, 2.0f )												;
	must->InterpoXPos( must->GetXpos(), must->GetXpos() - GetUniXpos( 50.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnRemove()
{
	remove->Enable()																		;
	remove->SetUpLeftPos( GetUniXpos( 427.f ), GetUniYpos( 7.f+30.f ) )						;
	remove->InterpoAlpha( 0.0f, 1.0f, 2.0f )												;
	remove->InterpoYPos( remove->GetYpos(), remove->GetYpos() - GetUniYpos( 40.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnMy2()
{
	my2->Enable()																	;
	my2->SetUpLeftPos( GetUniXpos( 184.f+40.f ), GetUniYpos( 90.f ) )				;
	my2->InterpoAlpha( 0.0f, 1.0f, 2.0f )											;
	my2->InterpoXPos( my2->GetXpos(), my2->GetXpos() - GetUniXpos( 80.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnSkin()
{
	skin->Enable()																		;
	skin->SetUpLeftPos( GetUniXpos( 306.f-20.f ), GetUniYpos( 100.f ) )					;
	skin->InterpoAlpha( 0.0f, 1.0f, 2.0f )												;
	skin->InterpoXPos( skin->GetXpos(), skin->GetXpos() + GetUniXpos( 40.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnTo()
{
	to->Enable()																;
	to->SetUpLeftPos( GetUniXpos( 417.f ), GetUniYpos( 116.f-40.f ) )			;
	to->InterpoAlpha( 0.0f, 1.0f, 2.0f )										;
	to->InterpoYPos( to->GetYpos(), to->GetYpos() + GetUniYpos( 60.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnSee()
{
	see->Enable()																	;
	see->SetUpLeftPos( GetUniXpos( 487.f+30.f ), GetUniYpos( 96.f ) )				;
	see->InterpoAlpha( 0.0f, 1.0f, 2.0f )											;
	see->InterpoXPos( see->GetXpos(), see->GetXpos() - GetUniXpos( 60.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnBelief()
{
	belief->Enable()																		;
	belief->SetUpLeftPos( GetUniXpos( 403.f-40.f ), GetUniYpos( 192.f ) )					;
	belief->InterpoAlpha( 0.0f, 1.0f, 2.0f )												;
	belief->InterpoXPos( belief->GetXpos(), belief->GetXpos() + GetUniXpos( 80.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnIn()
{
	in->Enable()																;
	in->SetUpLeftPos( GetUniXpos( 549.f ), GetUniYpos( 183.f+40.f ) )			;
	in->InterpoAlpha( 0.0f, 1.0f, 2.0f )										;
	in->InterpoYPos( in->GetYpos(), in->GetYpos() - GetUniYpos( 80.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnYour()
{
	your->Enable()																		;
	your->SetUpLeftPos( GetUniXpos( 451.f+20.f ), GetUniYpos( 261.f ) )					;
	your->InterpoAlpha( 0.0f, 1.0f, 2.0f )												;
	your->InterpoXPos( your->GetXpos(), your->GetXpos() - GetUniXpos( 40.f ), 9.0f )	;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::OnEyes()
{
	eyes->Enable()													;
	eyes->SetUpLeftPos( GetUniXpos( 431.f ), GetUniYpos( 326.f ) )	;
	eyes->InterpoAlpha( 0.0f, 1.0f, 1.0f )							;
	eyes->InterpoZoom( .5f, 2.f, 5.f )								;
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::Disappear( UImage* ima, Ufloat time )
{
	if( ima->IsEnable() )
	{
		if( !ima->IsInterpoAlpha() )
		{
			if( ima->GetAlpha() >= .99f )
				ima->InterpoAlpha( 1.0f, 0.0f, time )	;
			else
				ima->Disable()	;
		}
	}
}
//----------------------------------------------------------------------------------------------------------
void NebuTxt::Affiche( Ufloat laptime )
{
	Forsaken1->Affiche( laptime )	;
	Disappear( Forsaken1, 12.0f )	;

	by1->Affiche( laptime )	;
	Disappear( by1, 11.0f )	;

	destiny->Affiche( laptime )		;
	Disappear( destiny, 10.5f )		;

	Forsaken2->Affiche( laptime )	;
	Disappear( Forsaken2, 8.0f )	;

	by2->Affiche( laptime )	;
	Disappear( by2, 7.5f )	;

	my1->Affiche( laptime )	;
	Disappear( my1, 7.0f )	;

	own->Affiche( laptime )	;
	Disappear( own, 7.0f )	;

	mind->Affiche( laptime )	;
	Disappear( mind, 7.0f )		;

	I->Affiche( laptime )	;
	Disappear( I, 7.0f )	;

	must->Affiche( laptime )	;
	Disappear( must, 7.0f )		;

	remove->Affiche( laptime )	;
	Disappear( remove, 7.0f )	;

	my2->Affiche( laptime )	;
	Disappear( my2, 7.0f )	;

	skin->Affiche( laptime )	;
	Disappear( skin, 7.0f )		;

	to->Affiche( laptime )	;
	Disappear( to, 7.0f )	;

	see->Affiche( laptime )	;
	Disappear( see, 7.0f )	;

	belief->Affiche( laptime )	;
	Disappear( belief, 7.0f )	;

	in->Affiche( laptime )		;
	Disappear( in, 7.0f )		;

	your->Affiche( laptime )	;
	Disappear( your, 7.0f )		;

	eyes->Affiche( laptime )	;
	Disappear( eyes, 7.0f )		;
}
//----------------------------------------------------------------------------------------------------------
