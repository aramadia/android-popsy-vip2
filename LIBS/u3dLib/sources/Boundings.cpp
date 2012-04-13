    				 /*-----------------------------------------------------+
					  |				         Boundings.cpp					|
					  |														|
					  |		les machins ki entourent les des trucs 			|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/


#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								+---------------------------+
//								|    Class BoundingBox		|
//								----------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox()
{
	vec3_set( pts[0], +1e+19f, -1e+19f, -1e+19f )	;
	vec3_set( pts[1], -1e+19f, -1e+19f, -1e+19f )	;
	vec3_set( pts[2], +1e+19f, -1e+19f, +1e+19f )	;
	vec3_set( pts[3], -1e+19f, -1e+19f, +1e+19f )	;
	vec3_set( pts[4], +1e+19f, +1e+19f, -1e+19f )	;
	vec3_set( pts[5], -1e+19f, +1e+19f, -1e+19f )	;
	vec3_set( pts[6], +1e+19f, +1e+19f, +1e+19f )	;
	vec3_set( pts[7], -1e+19f, +1e+19f, +1e+19f )	;
}
//----------------------------------------------------------------------------------------------------------
// actualise la Bounding en fct du point si necessaire
void BoundingBox::TestEtActua( Ufloat *pt )
{
	if( pt[0] > pts[1][0] )
	{
		pts[1][0] = pt[0]	; 
		pts[3][0] = pt[0]	; 
		pts[5][0] = pt[0]	; 
		pts[7][0] = pt[0]	; 
	}
	if( pt[0] < pts[0][0] )
	{
		pts[0][0] = pt[0]	; 
		pts[2][0] = pt[0]	; 
		pts[4][0] = pt[0]	; 
		pts[6][0] = pt[0]	; 
	}
	if( pt[1] > pts[0][1] )
	{
		pts[0][1] = pt[1]	; 
		pts[1][1] = pt[1]	; 
		pts[2][1] = pt[1]	; 
		pts[3][1] = pt[1]	; 
	}
	if( pt[1] < pts[4][1] )
	{
		pts[4][1] = pt[1]	; 
		pts[5][1] = pt[1]	; 
		pts[6][1] = pt[1]	;
		pts[7][1] = pt[1]	; 
	}
	if( pt[2] > pts[0][2] )
	{
		pts[0][2] = pt[2]	; 
		pts[1][2] = pt[2]	; 
		pts[4][2] = pt[2]	; 
		pts[5][2] = pt[2]	; 
	}
	if( pt[2] < pts[2][2] )
	{
		pts[2][2] = pt[2]	;	 
		pts[3][2] = pt[2]	;	 
		pts[6][2] = pt[2]	;	 
		pts[7][2] = pt[2]	; 
	}
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si &b est contune dans la bounding box !
bool BoundingBox::Contenu( BoundingBox &b )
{
	if( pts[1][0] < b.pts[1][0] ) return FALSE	;
	if( pts[0][0] > b.pts[0][0] ) return FALSE	;

	if( pts[0][1] < b.pts[0][1] ) return FALSE	;
	if( pts[4][1] > b.pts[1][1] ) return FALSE	;

	if( pts[0][2] < b.pts[0][2] ) return FALSE	;
	if( pts[2][2] > b.pts[2][2] ) return FALSE	;

	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------
void BoundingBox::Set( AABoundingBox& b,  Mat3x4& m )
{
	Ufloat pos[3]	;

	vec3_set( pos, b.GetMinX(), b.GetMaxY(), b.GetMaxZ() )	;
	m.GetTransfo( pts[0], pos )								;

	vec3_set( pos, b.GetMaxX(), b.GetMaxY(), b.GetMaxZ() )	;
	m.GetTransfo( pts[1], pos )								;

	vec3_set( pos, b.GetMinX(), b.GetMaxY(), b.GetMinZ() )	;
	m.GetTransfo( pts[2], pos )								;

	vec3_set( pos, b.GetMaxX(), b.GetMaxY(), b.GetMinZ() )	;
	m.GetTransfo( pts[3], pos )								;

	vec3_set( pos, b.GetMinX(), b.GetMinY(), b.GetMaxZ() )	;
	m.GetTransfo( pts[4], pos )								;

	vec3_set( pos, b.GetMaxX(), b.GetMinY(), b.GetMaxZ() )	;
	m.GetTransfo( pts[5], pos )								;

	vec3_set( pos, b.GetMinX(), b.GetMinY(), b.GetMinZ() )	;
	m.GetTransfo( pts[6], pos )								;

	vec3_set( pos, b.GetMaxX(), b.GetMinY(), b.GetMinZ() )	;
	m.GetTransfo( pts[7], pos )								;
}
//----------------------------------------------------------------------------------------------------------
void BoundingBox::Affiche( Ufloat RVBA[4] )
{
	CoordU3D pt1,pt2		;
	Ufloat	 tRVBA[2][4]	;

	vec4_eg( tRVBA[0], RVBA )	;
	vec4_eg( tRVBA[1], RVBA )	;

	vec3_eg( pt1.trans, pts[0] )								;
	vec3_eg( pt2.trans, pts[1] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[0] )								;
	vec3_eg( pt2.trans, pts[2] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[0] )								;
	vec3_eg( pt2.trans, pts[4] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[3] )								;
	vec3_eg( pt2.trans, pts[1] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[3] )								;
	vec3_eg( pt2.trans, pts[2] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[3] )								;
	vec3_eg( pt2.trans, pts[7] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[6] )								;
	vec3_eg( pt2.trans, pts[7] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[6] )								;
	vec3_eg( pt2.trans, pts[2] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[6] )								;
	vec3_eg( pt2.trans, pts[4] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[5] )								;
	vec3_eg( pt2.trans, pts[4] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[5] )								;
	vec3_eg( pt2.trans, pts[7] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;

	pt1.frame_process = (U32)-1									;
	pt2.frame_process = (U32)-1									;
	vec3_eg( pt1.trans, pts[5] )								;
	vec3_eg( pt2.trans, pts[1] )								;
	U3D3Pipeline->AfficheLigneFromEngine( &pt1, &pt2, tRVBA )	;
}
//----------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								+---------------------------------+
//								|    Class BoundingSphere		  |
//								+---------------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
BoundingSphere::BoundingSphere()
{
	vec3_set( centre, 0.f, 0.f, 0.f )	;
	rayon = 0.f							;
	rayonFake = 0.f						;
}
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								+---------------------------+
//								|    Class AABoundingBox	|
//								----------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
AABoundingBox::AABoundingBox()
{
	vec3_set( Min, +1e+29f, +1e+29f, +1e+29f )	;	// set les minimums avec de tres grandes valeurs
	vec3_set( Max, -1e+29f, -1e+29f, -1e+29f )	;	// set les maximums avec de tres petites valeurs
}
//----------------------------------------------------------------------------------------------------------
void AABoundingBox::Reset()
{
	vec3_set( Min, +1e+29f, +1e+29f, +1e+29f )	;	// set les minimums avec de tres grandes valeurs
	vec3_set( Max, -1e+29f, -1e+29f, -1e+29f )	;	// set les maximums avec de tres petites valeurs
}
//----------------------------------------------------------------------------------------------------------
AABoundingBox& AABoundingBox::operator+=( AABoundingBox& b )
{
	for( U32 a=0; a<3; a++ )
	{
		if( Min[a] > b.Min[a] )	Min[a] = b.Min[a]	;
		if( Max[a] < b.Max[a] ) Max[a] = b.Max[a]	;
	}

	return *this	;
}
//----------------------------------------------------------------------------------------------------------
// indic ke le point est dans la bounding ( donc va egrandir la boundig si besoin )
void AABoundingBox::PointIn( const Ufloat pt[3] )
{
	for( U32 a=0; a<3; a++ )
	{
		if( Min[a] > pt[a] )	Min[a] = pt[a]	;
		if( Max[a] < pt[a] )	Max[a] = pt[a]	;
	}
}
//----------------------------------------------------------------------------------------------------------
// indic ke la valeur pt est dans la bounding en X ( donc va agrandir la boundig si besoin )
void AABoundingBox::PointInX( Ufloat pt )
{
	if( Min[0] > pt )	Min[0] = pt	;
	if( Max[0] < pt )	Max[0] = pt	;
}
//----------------------------------------------------------------------------------------------------------
// indic ke la valeur pt est dans la bounding en Y ( donc va agrandir la boundig si besoin )
void AABoundingBox::PointInY( Ufloat pt )
{
	if( Min[1] > pt )	Min[1] = pt	;
	if( Max[1] < pt )	Max[1] = pt	;
}
//----------------------------------------------------------------------------------------------------------
// indic ke la valeur pt est dans la bounding en Z ( donc va agrandir la boundig si besoin )
void AABoundingBox::PointInZ( Ufloat pt )
{
	if( Min[2] > pt )	Min[2] = pt	;
	if( Max[2] < pt )	Max[2] = pt	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si point a l'interieur de la bounding
bool AABoundingBox::IsInside( const Ufloat pt[3] )
{
	for( U32 a=0; a<3; a++ )
	{
		if( Min[a] > pt[a] )	return FALSE	;
		if( Max[a] < pt[a] )	return FALSE	;
	}

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si la bounding box est completement contenue dans la sphere
bool AABoundingBox::IsInside( const Ufloat pos[3], Ufloat rayonV )
{
/*	for( U32 a=0; a<3; a++ )
		if( (pos[a]-rayon > Min[a]) || (pos[a]+rayon < Max[a] ) ) return FALSE	;*/

	Ufloat tmp[3]					;
	Ufloat rayon = rayonV*rayonV	;

	vec3_set( tmp, Min[0], Min[1], Min[2] )						;
	if( vec3_distFake( tmp, pos) > rayon	)	return FALSE	;
	vec3_set( tmp, Max[0], Min[1], Min[2] )						;
	if( vec3_distFake( tmp, pos) > rayon	)	return FALSE	;
	vec3_set( tmp, Min[0], Max[1], Min[2] )						;
	if( vec3_distFake( tmp, pos) > rayon	)	return FALSE	;
	vec3_set( tmp, Max[0], Max[1], Min[2] )						;
	if( vec3_distFake( tmp, pos) > rayon	)	return FALSE	;
	vec3_set( tmp, Min[0], Min[1], Max[2] )						;
	if( vec3_distFake( tmp, pos) > rayon	)	return FALSE	;
	vec3_set( tmp, Max[0], Min[1], Max[2] )						;
	if( vec3_distFake( tmp, pos) > rayon	)	return FALSE	;
	vec3_set( tmp, Min[0], Max[1], Max[2] )						;
	if( vec3_distFake( tmp, pos) > rayon	)	return FALSE	;
	vec3_set( tmp, Max[0], Max[1], Max[2] )						;
	if( vec3_distFake( tmp, pos) > rayon	)	return FALSE	;


	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe la boundig box
// TODO : trouver une facon encore + rapide de la faiiiiiiiiiire !!!
bool AABoundingBox::SegmentIntersec( const Ufloat a[3], const Ufloat b[3] )
{
	Ufloat	scale						;
	Ufloat	tmpa[3], tmpb[3], tmpc[3]	;
	U32		i							;

	vec3_eg( tmpa, a )	;
	vec3_eg( tmpb, b )	;


	//------------------ va tester et clipper succesivement le segment par tout les coté de la bounding box

	for( i=0; i<3; i++ )
	{
		if( (tmpa[i]>= Max[i]) &&  (tmpb[i]>= Max[i]) )
			return FALSE	;	// segment definitivemnt hors de la bounding

		if( !( (tmpa[i]<Max[i]) && (tmpb[i]<Max[i]) ) )
		{
			if( tmpa[i]>Max[i] )
			{
				scale = ( Max[i] - tmpa[i] ) / ( tmpb[i] - tmpa[i] )	;
				vec3_sub( tmpc, tmpb, tmpa )							;
				vec3_mul( tmpc, tmpc, scale )							;
				vec3_add( tmpa, tmpc, tmpa )							;
			}
			else
			{
				scale = ( Max[i] - tmpb[i] ) / ( tmpa[i] - tmpb[i] )	;
				vec3_sub( tmpc, tmpa, tmpb )							;
				vec3_mul( tmpc, tmpc, scale )							;
				vec3_add( tmpb, tmpc, tmpb )							;
			}
		}

		if( (tmpa[i]<= Min[i]) &&  (tmpb[i]<= Min[i]) )
			return FALSE	;	// segment definitivemnt hors de la bounding

		if( !( (tmpa[i]>Min[i]) && (tmpb[i]>Min[i]) ) )
		{
			if( tmpa[i]<Min[i] )
			{
				scale = ( Min[i] - tmpa[i] ) / ( tmpb[i] - tmpa[i] )	;
				vec3_sub( tmpc, tmpb, tmpa )							;
				vec3_mul( tmpc, tmpc, scale )							;
				vec3_add( tmpa, tmpc, tmpa )							;
			}
			else
			{
				scale = ( Min[i] - tmpb[i] ) / ( tmpa[i] - tmpb[i] )	;
				vec3_sub( tmpc, tmpa, tmpb )							;
				vec3_mul( tmpc, tmpc, scale )							;
				vec3_add( tmpb, tmpc, tmpb )							;
			}
		}

	}

	return TRUE	;	// il reste toujours un segment donc c'est kil coupe bien la bounding box
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si la sphere coupe la bounding box de l'objet. ( NB rayon = vrai rayo*vrai rayon )
bool AABoundingBox::SphereIntersec( const Ufloat pos[3], const Ufloat rayon )
{
	for( U32 a=0; a<3; a++ )
	{
		if( pos[a] > Max[a] )
		{
			if( pos[a]-Max[a] > rayon ) return FALSE	;
		}
		else if( pos[a] < Min[a] )
		{
			if( Min[a]-pos[a] > rayon ) return FALSE	;
		}
	}

	return TRUE	;
/*	Ufloat tmp[3]	;

	vec3_set( tmp, Min[0], Min[1], Min[2] )					;
	if( vec3_distFake( tmp, pos) <= rayon	)	return TRUE	;
	vec3_set( tmp, Max[0], Min[1], Min[2] )					;
	if( vec3_distFake( tmp, pos) <= rayon	)	return TRUE	;
	vec3_set( tmp, Min[0], Max[1], Min[2] )					;
	if( vec3_distFake( tmp, pos) <= rayon	)	return TRUE	;
	vec3_set( tmp, Max[0], Max[1], Min[2] )					;
	if( vec3_distFake( tmp, pos) <= rayon	)	return TRUE	;
	vec3_set( tmp, Min[0], Min[1], Max[2] )					;
	if( vec3_distFake( tmp, pos) <= rayon	)	return TRUE	;
	vec3_set( tmp, Max[0], Min[1], Max[2] )					;
	if( vec3_distFake( tmp, pos) <= rayon	)	return TRUE	;
	vec3_set( tmp, Min[0], Max[1], Max[2] )					;
	if( vec3_distFake( tmp, pos) <= rayon	)	return TRUE	;
	vec3_set( tmp, Max[0], Max[1], Max[2] )					;
	if( vec3_distFake( tmp, pos) <= rayon	)	return TRUE	;


	return FALSE	;*/
}
//----------------------------------------------------------------------------------------------------------
// renvoie position de la sphere par rapport a la bounding
U32 AABoundingBox::SpherePos( const Ufloat pos[3], const Ufloat rayon )
{
	Ufloat	tmp[3]				;
	bool	Binside	= TRUE		;
	bool	Boutisde = TRUE	;

	vec3_set( tmp, Min[0], Min[1], Min[2] )	;
	if( vec3_distFake( tmp, pos) > rayon	)
		Binside = FALSE		;
	else
		Boutisde = FALSE	;

	vec3_set( tmp, Max[0], Min[1], Min[2] )	;
	if( vec3_distFake( tmp, pos) > rayon	)
		Binside = FALSE		;
	else
		Boutisde = FALSE	;

	vec3_set( tmp, Min[0], Max[1], Min[2] )	;
	if( vec3_distFake( tmp, pos) > rayon	)
		Binside = FALSE		;
	else
		Boutisde = FALSE	;

	vec3_set( tmp, Max[0], Max[1], Min[2] )	;
	if( vec3_distFake( tmp, pos) > rayon	)
		Binside = FALSE		;
	else
		Boutisde = FALSE	;

	vec3_set( tmp, Min[0], Min[1], Max[2] )	;
	if( vec3_distFake( tmp, pos) > rayon	)
		Binside = FALSE		;
	else
		Boutisde = FALSE	;

	vec3_set( tmp, Max[0], Min[1], Max[2] )	;
	if( vec3_distFake( tmp, pos) > rayon	)
		Binside = FALSE		;
	else
		Boutisde = FALSE	;

	vec3_set( tmp, Min[0], Max[1], Max[2] )	;
	if( vec3_distFake( tmp, pos) > rayon	)
		Binside = FALSE		;
	else
		Boutisde = FALSE	;

	vec3_set( tmp, Max[0], Max[1], Max[2] )	;
	if( vec3_distFake( tmp, pos) > rayon	)
		Binside = FALSE		;
	else
		Boutisde = FALSE	;

	if( Binside ) return	inside	;
	if( Boutisde ) return	outside	;
	return coupe					;
}
//----------------------------------------------------------------------------------------------------------
