	   				 /*-------------------------------------------------------------------------+
					  |								CilsCarotus.cpp								|
					  |																			|
					  |	Pourkoa carotus me direz-vous ? ben en fait paskeu c'est l'image de la  |
					  |				molecule de du pigment de la carote vala !					|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#include "stdafx.h"


//-----------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|   Class CilsCarotus	|
//										+-----------------------+
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------------------------------
CilsCarotus::CilsCarotus()
{
	carotus1 = new UImage( "Datas/meshes/Cils/carotus.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE)	;
	carotus1->SetTransp( MatTranspAddAlpha )														;
	carotus1->Disable()																				;
	Bcarotus1 = FALSE																				;

	carotus2 = new UImage		;
	(*carotus2) = (*carotus1)	;
	carotus2->Disable()			;
	Bcarotus2 = FALSE			;

	carotus3 = new UImage		;
	(*carotus3) = (*carotus1)	;
	carotus3->Disable()			;
	Bcarotus3 = FALSE			;

	carotus4 = new UImage		;
	(*carotus4) = (*carotus1)	;
	carotus4->Disable()			;
	Bcarotus4 = FALSE			;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
CilsCarotus::~CilsCarotus()
{
	delete carotus1	;
	delete carotus2	;
	delete carotus3	;
	delete carotus4	;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsCarotus::Go()
{
	carotus1->SetPos( Random1(), Random1() )				;
	carotus1->InterpoAlpha( 0.0f, 1.0f, Random1() + 1.0f )	;
	carotus1->InterpoZoom( Random1(), Random1()*2.f, 3.f )	;
	carotus1->Enable()										;
	Bcarotus1 = TRUE										;

	carotus2->SetPos( Random1(), Random1() )				;
	carotus2->InterpoAlpha( 0.0f, 1.0f, Random1() + 1.0f )	;
	carotus2->InterpoZoom( Random1(), Random1()*2.f, 3.f )	;
	carotus2->Enable()										;
	Bcarotus2 = TRUE										;

	carotus3->SetPos( Random1(), Random1() )				;
	carotus3->InterpoAlpha( 0.0f, 1.0f, Random1() + 1.0f )	;
	carotus3->InterpoZoom( Random1(), Random1()*2.f, 3.f )	;
	carotus3->Enable()										;
	Bcarotus3 = TRUE										;

	carotus4->SetPos( Random1(), Random1() )				;
	carotus4->InterpoAlpha( 0.0f, 1.0f, Random1() + 1.0f )	;
	carotus4->InterpoZoom( Random1(), Random1()*2.f, 3.f )	;
	carotus4->Enable()										;
	Bcarotus4 = TRUE										;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsCarotus::UpdateFrame( Ufloat laptime )
{
	carotus1->Affiche( laptime )	;
	if( Bcarotus1 )
	{
		if( !carotus1->IsInterpoAlpha() )
		{
			Bcarotus1 = FALSE							;
			carotus1->InterpoAlpha( 1.0f, 0.0f, 1.0f )	;
		}
	}
	else if( !carotus1->IsInterpoAlpha() )
	{
		carotus1->Disable()	;
	}


	carotus2->Affiche( laptime )	;
	if( Bcarotus2 )
	{
		if( !carotus2->IsInterpoAlpha() )
		{
			Bcarotus2 = FALSE							;
			carotus2->InterpoAlpha( 1.0f, 0.0f, 1.0f )	;
		}
	}
	else if( !carotus2->IsInterpoAlpha() )
	{
		carotus2->Disable()	;
	}

	carotus3->Affiche( laptime )	;
	if( Bcarotus3 )
	{
		if( !carotus3->IsInterpoAlpha() )
		{
			Bcarotus3 = FALSE							;
			carotus3->InterpoAlpha( 1.0f, 0.0f, 1.0f )	;
		}
	}
	else if( !carotus3->IsInterpoAlpha() )
	{
		carotus3->Disable()	;
	}

	carotus4->Affiche( laptime )	;
	if( Bcarotus4 )
	{
		if( !carotus4->IsInterpoAlpha() )
		{
			Bcarotus4 = FALSE							;
			carotus4->InterpoAlpha( 1.0f, 0.0f, 1.0f )	;
		}
	}
	else if( !carotus4->IsInterpoAlpha() )
	{
		carotus4->Disable()	;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
