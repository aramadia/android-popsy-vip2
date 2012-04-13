    				 /*-----------------------------------------------------+
					  |				         quat.cpp		    		    |
					  |														|
					  |	  Une chtite class qui gère les quaternions et tout |
					  |				se qui va avec bien sûr					|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
//									+---------------------+
//									|    Les Fonctions    |
//									+---------------------+
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
// créé le quaternion à partir d'un axeet d'un angle sur cet axe
Quat::Quat(Ufloat v[3], Ufloat angle)
{
	vec3_eg( q, v )				;
	vec3_normalize( q )			;
	angle *= .5					;
	vec3_mul( q, q, sin(angle) );
	q[3] = cos(angle)			;
}
//----------------------------------------------------------------------------------------------------------
void Quat::SetFromAxis( Ufloat v1, Ufloat v2, Ufloat v3, Ufloat angle )
{
	vec3_set( q, v1, v2, v3 )	;
	vec3_normalize( q )			;
	angle *= .5					;
	vec3_mul( q, q, sin(angle) );
	q[3] = cos(angle)			;
}
//----------------------------------------------------------------------------------------------------------
Quat Quat::operator=( Ufloat srce[4] )
{
	memcpy( q, srce, sizeof(q) )	;
	return *this					;
}
//----------------------------------------------------------------------------------------------------------
Quat Quat::operator=( Quat srce )
{
	memcpy( q, srce.q, sizeof(q) )	;
	return *this					;
}
//----------------------------------------------------------------------------------------------------------
Quat Quat::operator-( Quat& srce )
{
	Quat ret= *this	;

	ret.q[0]-=srce.q[0]	;
	ret.q[1]-=srce.q[1]	;
	ret.q[2]-=srce.q[2]	;
	ret.q[3]-=srce.q[3]	;

	return ret;
}
//----------------------------------------------------------------------------------------------------------
Quat Quat::operator+( Quat &srce )
{
	Quat ret= *this	;

	ret.q[0]+=srce.q[0]	;
	ret.q[1]+=srce.q[1]	;
	ret.q[2]+=srce.q[2]	;
	ret.q[3]+=srce.q[3]	;

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
Quat Quat::operator+=( Quat& srce )
{
	q[0]+=srce.q[0]	;
	q[1]+=srce.q[1]	;
	q[2]+=srce.q[2]	;
	q[3]+=srce.q[3]	;

	return *this	;
}
//----------------------------------------------------------------------------------------------------------
Quat Quat::operator*( Quat &srce )
{
	Quat	ret			;

	ret.q[0] = q[3] * srce[0] + q[0] * srce[3] + q[1] * srce[2] - q[2] * srce[1]	;
	ret.q[1] = q[3] * srce[1] + q[1] * srce[3] + q[2] * srce[0] - q[0] * srce[2]	;
	ret.q[2] = q[3] * srce[2] + q[2] * srce[3] + q[0] * srce[1] - q[1] * srce[0]	;
	ret.q[3] = q[3] * srce[3] - q[0] * srce[0] - q[1] * srce[1] - q[2] * srce[2]	;

	ret.normalize()	;

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
Quat& Quat::operator*=( Quat srce )
{
	Ufloat	tmp[4]	;

	tmp[0] = q[3] * srce[0] + q[0] * srce[3] + q[1] * srce[2] - q[2] * srce[1]	;
	tmp[1] = q[3] * srce[1] + q[1] * srce[3] + q[2] * srce[0] - q[0] * srce[2]	;
	tmp[2] = q[3] * srce[2] + q[2] * srce[3] + q[0] * srce[1] - q[1] * srce[0]	;
	tmp[3] = q[3] * srce[3] - q[0] * srce[0] - q[1] * srce[1] - q[2] * srce[2]	;

	q[0] = tmp[0]	;
	q[1] = tmp[1]	;
	q[2] = tmp[2]	;
	q[3] = tmp[3]	;

	normalize()	;

	return *this	;
}
//----------------------------------------------------------------------------------------------------------
Quat Quat::operator*( Ufloat f )
{
	Quat	ret = *this	;

	ret.q[0] *= f	;
	ret.q[1] *= f	;
	ret.q[2] *= f	;
	ret.q[3] *= f	;

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
void Quat::normalize()
{
	Ufloat mag;

	if( ( mag = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3] ) !=0 )
		mag = 1/sqrt(mag)	;

	vec3_mul( q, q, mag )	;	q[3] *= mag	;
}
//----------------------------------------------------------------------------------------------------------
void Quat::neg()
{
	q[0] = -q[0]	;
	q[1] = -q[1]	;
	q[2] = -q[2]	;
	q[3] = -q[3]	;
}
//----------------------------------------------------------------------------------------------------------
void Quat::conj()
{
	q[0] = -q[0]	;
	q[1] = -q[1]	;
	q[2] = -q[2]	;
	q[3] =  q[3]	;

}
//----------------------------------------------------------------------------------------------------------
void Quat::inv()
{
	Quat c=*this	;
	Quat ret		;
	Ufloat imag		;

	c.conj()											;
	imag = 1/(q[0]*q[0]+q[1]*q[1]+q[2]+q[2]+q[3]*q[3])	;
	vec3_mul( q, c.q, imag )							;
	q[3] = c.q[3]*imag									;

}
//----------------------------------------------------------------------------------------------------------
void Quat::ln()
{
	Ufloat  theta,mulf,mag	;

	mag   = vec3_sqrt(q)			;
	theta = atan2(mag,q[3])			;
	if( mag>0.0f ) mulf = theta/mag	;
	else		   mulf = +1e+19f	;
	vec3_mul( q, q, mulf)			;
	q[3]=0.0f						;
}
//----------------------------------------------------------------------------------------------------------
void Quat::exp()
{
	Ufloat	theta,cv,sv	;

	theta = vec3_sqrt(q)		;
	cv    = cos(theta)			;
	sv	  = sin(theta)/theta	;
	vec3_mul( q, q, sv )		;		
	q[3]=cv						;
}
//----------------------------------------------------------------------------------------------------------
Ufloat Quat::dot( Quat &srce)
{
	return ( vec3_dot(q,srce.q) + q[3]*srce.q[3] )	;
}
//----------------------------------------------------------------------------------------------------------
#define DELTA 1e-6     // error tolerance
Quat Quat::slerp( const Quat &to, GLfloat t )
{
	Ufloat		to1[4]				;
	Udouble     omega, cosom, sinom	;
	Udouble     scale0, scale1		;
	Quat		ret					;

        // calc cosine
        cosom = q[0] * to.q[0] + q[1] * to.q[1] + q[2] * to.q[2] + q[3] * to.q[3]	;

        // adjust signs (if necessary)
        if ( cosom < 0.0 )
		{
			cosom = -cosom	;

			to1[0] = - to.q[0]	;
			to1[1] = - to.q[1]	;
			to1[2] = - to.q[2]	;
			to1[3] = - to.q[3]	;

        } else  {

			to1[0] = to.q[0]	;
			to1[1] = to.q[1]	;
			to1[2] = to.q[2]	;
			to1[3] = to.q[3]	;

        }

        // calculate coefficients
        if ( (1.0 - cosom) > DELTA ) 
		{
                // standard case (slerp)
                omega = acos(cosom)						;
                sinom = sin(omega)						;
                scale0 = sin((1.0 - t) * omega) / sinom	;
                scale1 = sin(t * omega) / sinom			;

        } 
		else 
		{        
			    // "from" and "to" quaternions are very close 
			    //  ... so we can do a linear interpolation

                scale0 = 1.0 - t	;
                scale1 = t			;
        }

		// calculate final values
		ret.q[0] = scale0 * q[0] + scale1 * to1[0]	;
		ret.q[1] = scale0 * q[1] + scale1 * to1[1]	;
		ret.q[2] = scale0 * q[2] + scale1 * to1[2]	;
		ret.q[3] = scale0 * q[3] + scale1 * to1[3]	;

		return ret	;
}
//----------------------------------------------------------------------------------------------------------
void Quat::matrix( Ufloat m[4][4] )
{
	Ufloat	q0s=q[0]*q[0], q1s=q[1]*q[1], q2s=q[2]*q[2]	;
	Ufloat	q01=q[0]*q[1], q23=q[2]*q[3], q03=q[0]*q[3]	;
	Ufloat	q12=q[1]*q[2], q02=q[0]*q[2], q13=q[1]*q[3]	;

	m[0][0] = 1.0 - 2.0 * (q1s + q2s)	;
	m[1][0] = 2.0 * (q01 - q23)			;
	m[2][0] = 2.0 * (q02 + q13)			;
	m[3][0] = 0.0						;

	m[0][1] = 2.0 * (q01 + q23)			;
	m[1][1] = 1.0 - 2.0 * (q2s + q0s)	;
	m[2][1] = 2.0 * (q12 - q03)			;
	m[3][1] = 0.0						;

	m[0][2] = 2.0 * (q02 - q13)			;
	m[1][2] = 2.0 * (q12 + q03)			;
	m[2][2] = 1.0 - 2.0 * (q1s + q0s)	;
	m[3][2] = 0.0						;

	m[0][3] = 0.0;    m[1][3] = 0.0	;
	m[2][3] = 0.0;    m[3][3] = 1.0	;
}
//----------------------------------------------------------------------------------------------------------
void Quat::matrix( Mat3x4 &m )
{
	Ufloat	q0s=q[0]*q[0], q1s=q[1]*q[1], q2s=q[2]*q[2]	;
	Ufloat	q01=q[0]*q[1], q23=q[2]*q[3], q03=q[0]*q[3]	;
	Ufloat	q12=q[1]*q[2], q02=q[0]*q[2], q13=q[1]*q[3]	;

	m.Set( 1.0f - 2.0f * (q1s+q2s),        2.0f * (q01 + q23),        2.0f * (q02 - q13),
		        2.0f * (q01 - q23), 1.0f - 2.0f * (q2s + q0s),        2.0f * (q12 + q03),
			    2.0f * (q02 + q13),        2.0f * (q12 - q03), 1.0f - 2.0f * (q1s + q0s),
				       m.GetXpos(),               m.GetYpos(),               m.GetZpos()	)	;
}		
//----------------------------------------------------------------------------------------------------------
