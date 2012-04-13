    				 /*-----------------------------------------------------+
					  |				         Plan.cpp						|
					  |														|
					  |	   c'est fou comme les plans sont utils en 3D		|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									|    Class Plan		  |
//									+---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
Plan::Plan( Ufloat vec[3], Ufloat pos[3] )
{
	vec3_eg( normal, vec )			;
	vec3_normalize( normal )		;
	dist = vec3_dot( pos, normal )	;
}
//----------------------------------------------------------------------------------------------------------
// recalcul le plan à partie des 3 points 
void Plan::Calc( const Ufloat pt0[3], const Ufloat pt1[3], const Ufloat pt2[3] )
{
	Ufloat v1[3],v2[3];

	vec3_sub( v1, pt1, pt0 )		;
	vec3_sub( v2, pt2, pt0 )		;
	vec3_xprd( normal, v1, v2 )		;
	vec3_normalize( normal )		;
	dist = vec3_dot( pt0, normal )	;
}
//----------------------------------------------------------------------------------------------------------
//renvoie position d'une Bounding box par rapport au plan
/*BoundPlace Uplan_BSP::GetPos( BoundingBox &b )
{
	bool derriere=FALSE,devant=FALSE;

	for(U32 a=0; a<8; a++)
	{
		if( distance( b.pts[a] )< 0 )	derriere = TRUE;
		else devant = TRUE;
	}
	if( derriere && devant ) return ENTRE_DEUX;
	if( derriere ) return DERRIERE;
	else return DEVANT;
}*/
//----------------------------------------------------------------------------------------------------------
// renvoie la matrice de transformation associée au plan 
// NOTES : refaire fct avec utilisation d'un cross product au lieu de ce merdier immonde
void Plan::GetTransMatrix( Mat3x4& m )
{
	Ufloat r,rx,ry	;
	Mat3x4 t		;
	Ufloat v1[3]	;

	r = sqrt( normal[2]*normal[2] + normal[0]*normal[0] )	;
	if( r==0.0f )	// cas particulier
	{
		m.Identity()				;
		m.Set( 1, 0.f, 0.f, 1.f )	;
		m.Set( 2, 0.f, 1.f, 0.f )	;
		return						;
	}

	ry= acos( normal[2]/r )		;
	if( normal[0]<0 ) ry=-ry	;

	Ufloat vcos = Cos(ry)			;
	Ufloat vsin = Sin(ry)			;
	t.Set( 0, vcos, 0.0f, -vsin )	;
	t.Set( 2, vsin, 0.0f, vcos )	;
/*	t[0][0]=cos(ry);	t[0][2]=-sin(ry);
	t[2][0]=sin(ry);	t[2][2]=cos(ry);	*/

	v1[0] = t.GetXTransfoR( normal )	;
	v1[1] = t.GetYTransfoR( normal )	;
	v1[2] = t.GetZTransfoR( normal )	;

	r = sqrt( v1[2]*v1[2] + v1[1]*v1[1] )	;
	rx= acos( v1[2]/r )						;
	if( normal[1]>0 ) rx=-rx				;

	m.Identity()					;
	vcos = cos(rx)					;
	vsin = sin(rx)					;
	m.Set( 1, 0.0f, vcos, vsin )	;
	m.Set( 2, 0.0f, -vsin, vcos )	;
/*	m[1][1]=cos(rx);		m[1][2]=sin(rx);
	m[2][1]=-sin(rx);		m[2][2]=cos(rx);*/

//	m44_multiply(m,t,m);
	m *= t	;

	vec3_eg( v1, normal )		;
	vec3_mul( v1, v1, dist )	;

	m.SetPos( v1[0], v1[1], v1[2] )	;
}
//----------------------------------------------------------------------------------------------------------
// applique la matrice au plan 

// Depuis Crystal Space Engine :
// let N represent norm <A,B,C>, and X represent point <x,y,z>
//
// Old plane equation: N*X + D = 0
// There exists point X = <r*A,r*B,r*C> = r*N which satisfies the
// plane equation.  
//  => r*(N*N) + D = 0
//  => r = -D/(N*N)
//
// New plane equation: N'*X' + D' = 0
// If M is the transformation matrix, and V the transformation vector,
// N' = M*N, and X' = M*(X-V).  Assume that N' is already calculated.
//  => N'*(M*(X-V)) + D' = 0
//  => D' = -N'*(M*X) + N'*(M*V)
//        = -N'*(M*(r*N)) + N'*(M*V)
//        = -r*(N'*N') + N'*(M*V) = D*(N'*N')/(N*N) + N'*(M*V)
// Since N' is a rotation of N, (N'*N') = (N*N), thus
//  D' = D + N'*(M*V)
//
Plan& Plan::operator*=(Mat3x4 &m)
{
	Ufloat	tmp[3]		;
	Ufloat	v_ori[3]	;

	vec3_eg( tmp, normal )										;
	vec3_set( v_ori, m.GetXpos(), m.GetYpos(), m.GetZpos() )	;

	m.GetTransfoR( normal, tmp )	;
	m.GetTransfoR( tmp, v_ori )		;

	dist = -(-dist + vec3_dot( normal, tmp ) )	;
	

	return *this	;
}
//----------------------------------------------------------------------------------------------------------
