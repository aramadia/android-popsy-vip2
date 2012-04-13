    					 /*-----------------------------------------------------+
						  |				       NeroTexte.cpp					|
						  |														|
						  |	Affichage des giga paroles du chanteur de la chanson|
						  |														|
						  | 	U2^PoPsy TeAm 2000								|
						  +-----------------------------------------------------*/

#include "stdafx.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|    Class NeuroTxt	|
//										+-------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
NeuroTxt::NeuroTxt()
{
	TextureManager::SetRepertoire( "" )	;

	texture1 = TextureManager::OpenJpg( "Datas/meshes/neurones/Texte (1).jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE )	;

	There = new UImage								;
	There->SetTexture( texture1, 512.f, 256.f )		;
	There->SetCoordRV( 3.f, 220.f, 21.0f, 110.f )	;	// coordonnées a l'intérieur de la texture
	There->SetTransp( MatTranspAddAlpha )			;
	There->Disable()								;

	Was = new UImage								;
	Was->SetTexture( texture1, 512.f, 256.f )		;
	Was->SetCoordRV( 220.f, 386.f, 35.0f, 107.f )	;
	Was->SetTransp( MatTranspAddAlpha )				;
	Was->Disable()									;

	No = new UImage									;
	No->SetTexture( texture1, 512.f, 256.f )		;
	No->SetCoordRV( 393.f, 504.f, 35.0f, 107.f )	;
	No->SetTransp( MatTranspAddAlpha )				;
	No->Disable()									;

	God = new UImage								;
	God->SetTexture( texture1, 512.f, 256.f )		;
	God->SetCoordRV( 4.f, 195.f, 132.0f, 245.f )	;
	God->SetTransp( MatTranspAddAlpha )				;
	God->Disable()									;

	For = new UImage								;
	For->SetTexture( texture1, 512.f, 256.f )		;
	For->SetCoordRV( 195.f, 344.f, 128.0f, 238.f )	;
	For->SetTransp( MatTranspAddAlpha )				;
	For->Disable()									;

	Me = new UImage									;
	Me->SetTexture( texture1, 512.f, 256.f )		;
	Me->SetCoordRV( 347.f, 508.f, 143.0f, 235.f )	;
	Me->SetTransp( MatTranspAddAlpha )				;
	Me->Disable()									;

	Allthat	 = new UImage( "Datas/meshes/neurones/allthat.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Allthat->OpenMask( "Datas/meshes/neurones/allthat.raw" )										;
	Allthat->SetPos( Allthat->GetXSize()*.5f, 1.0f - Allthat->GetYSize()*.5f )						;
	Allthat->SetTransp( MatTranspAlpha )															;
	Allthat->Disable()																				;

	Shatters	 = new UImage( "Datas/meshes/neurones/shatters.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Shatters->OpenMask( "Datas/meshes/neurones/shatters.raw" )											;
	Shatters->SetPos( Shatters->GetXSize()*.5f, 1.0f - Shatters->GetYSize()*.5f )							;
	Shatters->SetTransp( MatTranspAlpha )																	;
	Shatters->Disable()																						;

	Absense		 = new UImage( "Datas/meshes/neurones/absence.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE )	;
	Absense->OpenMask( "Datas/meshes/neurones/absence.raw" )											;
	Absense->SetPos( Absense->GetXSize()*.5f, 1.0f - Absense->GetYSize()*.5f )							;
	Absense->SetTransp( MatTranspAlpha )																;
	Absense->Disable()																					;

	SloganPos = 0	;
}
//----------------------------------------------------------------------------------------------------------
NeuroTxt::~NeuroTxt()
{
	delete  There	;
	delete	Was		;
	delete	No		;
	delete	God		;
	delete	For		;
	delete	Me		;

	delete Allthat	;
	delete Shatters	;
	delete Absense	;

	TextureManager::SubUse( texture1 )	;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnThere()
{
	There->Enable()													;
	There->SetUpLeftPos( GetUniXpos( 269.f ), GetUniYpos( 20.f ) )	;
	There->InterpoAlpha( 0.0f, 1.0f, 2.0f )							;
	There->InterpoZoom( 2.0f, .2f, 4.0f )							;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnWas()
{
	Was->Enable()													;
	Was->SetUpLeftPos( GetUniXpos( 470.f ), GetUniYpos( 28.f ) )	;
	Was->InterpoAlpha( 0.0f, 1.0f, 2.0f )							;
	Was->InterpoZoom( 0.2f, 2.0f, 4.0f )							;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnNo()
{
	No->Enable()												;
	No->SetUpLeftPos( GetUniXpos( 338.f ), GetUniYpos( 88.f ) )	;
	No->InterpoAlpha( 0.0f, 1.0f, 2.0f )						;
	No->InterpoZoom( 2.0f, 0.2f, 4.0f )							;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnGod()
{
	God->Enable()													;
	God->SetUpLeftPos( GetUniXpos( 445.f ), GetUniYpos( 95.f ) )	;
	God->InterpoAlpha( 0.0f, 1.0f, 2.0f )							;
	God->InterpoZoom( 0.2f, 2.0f, 4.0f )							;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnFor()
{
	For->Enable()													;
	For->SetUpLeftPos( GetUniXpos( 297.f ), GetUniYpos( 143.f ) )	;
	For->InterpoAlpha( 0.0f, 1.0f, 2.0f )							;
	For->InterpoZoom( 2.0f, 0.2f, 4.0f )							;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnMe()
{
	Me->Enable()													;
	Me->SetUpLeftPos( GetUniXpos( 460.f ), GetUniYpos( 187.f ) )	;
	Me->InterpoAlpha( 0.0f, 1.0f, 2.0f )							;
	Me->InterpoZoom( 0.2f, 2.0f, 4.0f )								;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnShatters( Ufloat time )
{
	Shatters->InterpoAlpha( 1.0f, 0.0f, time )	;
	Shatters->Enable()							;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnAllthat( Ufloat time )
{
	Allthat->InterpoAlpha( 1.0f, 0.0f, time )	;
	Allthat->Enable()							;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::OnAbsense( Ufloat time )
{
	Absense->InterpoAlpha( 1.0f, 0.0f, time )	;
	Absense->Enable()							;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::Disappear( UImage* ima, Ufloat time )
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
void NeuroTxt::NextSlogan()
{
	switch( SloganPos )
	{
		case 0:
			OnAllthat( 1.0f )	;
			break;

		case 1:
			OnShatters( 1.0f )	;
			break;

		case 2:
			OnAbsense( 1.0f )	;
			break;
	}

	SloganPos++	;	SloganPos = SloganPos%3	;
}
//----------------------------------------------------------------------------------------------------------
void NeuroTxt::Affiche( Ufloat laptime )
{
	There->Affiche( laptime )	;
	Disappear( There, 2.0f )	;

	Was->Affiche( laptime )	;
	Disappear( Was, 2.0f )	;

	No->Affiche( laptime )	;
	Disappear( No, 2.0f )	;

	God->Affiche( laptime )	;
	Disappear( God, 2.0f )	;

	For->Affiche( laptime )	;
	Disappear( For, 2.0f )	;

	Me->Affiche( laptime )	;
	Disappear( Me, 2.0f )	;

	Shatters->Affiche( laptime )	;
	if( !Shatters->IsInterpoAlpha() )
		Shatters->Disable()	;

	Allthat->Affiche( laptime )	;
	if( !Allthat->IsInterpoAlpha() )
		Allthat->Disable()	;

	Absense->Affiche( laptime )	;
	if( !Absense->IsInterpoAlpha() )
		Absense->Disable()	;
}
//----------------------------------------------------------------------------------------------------------
