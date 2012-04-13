    				 /*-----------------------------------------------------+
					  |				         Hide.cpp						|
					  |														|
					  |	une classe toute simple ki va gérer les hide keys	|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/

#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+--------------------+
//									|    Class HideU3D   |
//									+--------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
HideU3D::HideU3D( HideKeyFileU3D *h, U32 nbh )
{
	HideStatus = FALSE	;
	nbKey = nbh			;
	actualNumKey = 0	;
	timePos = 0.0f		;

	Keys = new Ufloat[ nbh ]	;

	for( U32 a=0; a<nbh; a++ )
		Keys[a] = h[a].pos	;
}
//----------------------------------------------------------------------------------------------------------
HideU3D::~HideU3D()
{
	delete [] Keys	;
}
//----------------------------------------------------------------------------------------------------------
// remet la parcour de la spline au debut
void HideU3D::Ini()
{
	HideStatus = FALSE	;
	actualNumKey = 0	;
	timePos = 0.0f		;
}
//----------------------------------------------------------------------------------------------------------
// met la courbe a une position donnée
void HideU3D::SetPos( Ufloat p )
{
	Ini()		;
	IncPos( p )	;
}
//----------------------------------------------------------------------------------------------------------
// incrémente position dans la spline de n valeur
void HideU3D::IncPos( Ufloat n )
{
	bool sortie = FALSE	;

	timePos += n	;

	if( actualNumKey >= nbKey ) return	;

	while( !sortie )
	{
		if( Keys[ actualNumKey ] <= timePos ) 
		{
			HideStatus = !HideStatus	;
			actualNumKey++				;
			if( actualNumKey >= nbKey )	
				sortie = TRUE	;
		}
		else
		{
			sortie = TRUE	;
		}
	}
}
//----------------------------------------------------------------------------------------------------------
