					 /*-----------------------------------------------------------------+
					  |							LettresBack.cpp							|
					  |																	|
					  |	 un fake blur sur une jolie image fournit par taggy !!!!!		|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/


#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class LettresBack	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
LettresBack::LettresBack()
{
/*	image1	=new UImage( "Datas/meshes/Lettres/fond1.jpg", BILINEAR_TEXTURE, 512.f, 512.f )	;
	image1->SetPos( 0.5f, 0.5f )																;
	image1->SetSize( 1.0f )																		;

	image2	=new UImage( "Datas/meshes/Lettres/fond2.jpg", BILINEAR_TEXTURE, 512.f, 512.f )	;
	image2->SetPos( 0.5f, 0.5f )																;
	image2->SetTransp( MatTranspAlpha )															;
	image2->SetSize( 1.0f )																		;*/

	alpha = 0.0f		;
	alphadest = 0.0f	;
	incalpha = 0.0f		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
LettresBack::~LettresBack()
{
/*	delete image1	;
	delete image2	;*/
}
//----------------------------------------------------------------------------------------------------------------------------------------
void LettresBack::Interpo( Ufloat dep, Ufloat ariv, Ufloat time )
{
	alpha = dep							;
	alphadest = ariv					;
	incalpha = ( alphadest - dep )/time	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void LettresBack::UpdateFRame( Ufloat laptime )	
{
	alpha += laptime*incalpha	;

	if( incalpha>0.0f )
	{
		if( alpha>alphadest)
			alpha = alphadest	;
	}
	else
	{
		if( alpha<alphadest)
			alpha = alphadest	;
	}

/*	image2->SetAlpha( 1.0f - alpha )		;

	image1->Affiche( laptime )	;
	image2->Affiche( laptime )	;*/
}
//----------------------------------------------------------------------------------------------------------------------------------------
