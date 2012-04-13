    				 /*-----------------------------------------------------+
					  |				         quat.h			    		    |
					  |														|
					  |	  Une chtite class qui gère les quaternions et tout |
					  |				se qui va avec bien sûr					|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifndef _quat_U3D3_H
#define _quat_U3D3_H

class Quat{
	public:
	Ufloat q[4]	;

public:
	Quat()												{q[0]=0;q[1]=0;q[2]=0;q[3]=0;}
	Quat( Ufloat v1, Ufloat v2, Ufloat v3, Ufloat v4 )	{q[0]=v1;q[1]=v2;q[2]=v3;q[3]=v4;}
	Quat(Ufloat v[3], Ufloat angle)						;

	void	SetFromAxis( Ufloat v1, Ufloat v2, Ufloat v3, Ufloat angle )	;

	float	operator[]( U32 p )			{return q[p];}
	Quat	operator=( Ufloat srce[4] )	;
	Quat	operator=( Quat srce )		;
	Quat	operator-( Quat& srce )		;
	Quat	operator+( Quat& srce )		;
	Quat	operator+=( Quat& srce )	;
	Quat	operator*( Quat& srce )		;
	Quat&	operator*=( Quat srce )	;
	Quat	operator*( Ufloat f )		;

	void	normalize()							;
	void	neg()								;
	void	inv()								;
	void	conj()								;
	void	ln()								;
	void	exp()								;
	Ufloat	dot( Quat &srce )					;
	Quat	slerp( const Quat &to, GLfloat t )	;	// renvoie spherical interpolation 

	void matrix( Ufloat m[4][4] )	;
	void matrix( Mat3x4 &m )		;
};

#endif
