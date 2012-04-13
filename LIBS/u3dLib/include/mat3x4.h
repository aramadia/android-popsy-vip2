    				 /*-----------------------------------------------------+
					  |						Mat3x4.h						|
					  |														|
					  |	Mon implémentation des matrices de transformartions |
					  |					dans le moteur TRidé 				|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/


#ifndef _Mat3x4_U3D3_H_
#define _Mat3x4_U3D3_H_

#include <linuxcompat.h>

class Mat3x4{
public:
	float	m11,m12,m13	;
	float	m21,m22,m23	;
	float	m31,m32,m33	;
	float	xp,yp,zp	;

public:

	Mat3x4()													;
	Mat3x4( Ufloat m11, Ufloat m12, Ufloat m13,
			Ufloat m21, Ufloat m22, Ufloat m23,
			Ufloat m31, Ufloat m32, Ufloat m33,
			Ufloat xp=0.0f, Ufloat yp=0.0f, Ufloat zp=0.0f )	;

	void Serialize( CArchive &ar )	;

	void Set( Ufloat m11, Ufloat m12, Ufloat m13,
			Ufloat m21, Ufloat m22, Ufloat m23,
			Ufloat m31, Ufloat m32, Ufloat m33,
			Ufloat xp=0.0f, Ufloat yp=0.0f, Ufloat zp=0.0f )	;
	void Set( U32 ligne, Ufloat m1, Ufloat m2, Ufloat m3 )		;
	void SetPos( Ufloat x, Ufloat y, Ufloat z )					{xp=x;yp=y;zp=z;}
	void SetXPos( Ufloat x )									{xp=x;}
	void SetYPos( Ufloat y )									{yp=y;}
	void SetZPos( Ufloat z )									{zp=z;}
	void AddToPos( Ufloat x, Ufloat y, Ufloat z )				{xp+=x;yp+=y;zp+=z;}

	void	Identity()		;
	void	Transpose ()	;
	Mat3x4	GetInverse()	;
	float	Determinant()	;

//	Mat3x4& operator=(const Mat3x4& m)	;
	Mat3x4& operator*=(const Mat3x4& m)	;
	Mat3x4& operator*=(float s)			;

	float	GetXpos()												{return xp;}
	float	GetYpos()												{return yp;}
	float	GetZpos()												{return zp;}
	void	GetPos( Ufloat dest[3] )								{vec3_set(dest, xp, yp, zp);}
	float	GetXTransfo( const Ufloat v[3] )						{return v[0]*m11 + v[1]*m12 + v[2]*m13 + xp;}
	float	GetYTransfo( const Ufloat v[3] )						{return v[0]*m21 + v[1]*m22 + v[2]*m23 + yp;}
	float	GetZTransfo( const Ufloat v[3] )						{return v[0]*m31 + v[1]*m32 + v[2]*m33 + zp;}
	float	GetXTransfoR( const Ufloat v[3] )						{return v[0]*m11 + v[1]*m12 + v[2]*m13;}		// sans deplacement ( pour un vecteur )
	float	GetYTransfoR( const Ufloat v[3] )						{return v[0]*m21 + v[1]*m22 + v[2]*m23;}		// sans deplacement ( pour un vecteur )
	float	GetZTransfoR( const Ufloat v[3] )						{return v[0]*m31 + v[1]*m32 + v[2]*m33;}		// sans deplacement ( pour un vecteur )
	void	GetTransfo( Ufloat vdest[3], const Ufloat vsrce[3] )	;	// Transfo pour un point
	void	GetTransfoR( Ufloat vdest[3], const Ufloat vsrce[3] )	;	// Transfo pour un vecteur 
	void	GetTranspose( Ufloat vdest[3], const Ufloat vsrce[3] )	;
	void	GetTransposeR( Ufloat vdest[3], const Ufloat vsrce[3] )	;	// donne le transposé ( pour un vecteur )
};


#endif
