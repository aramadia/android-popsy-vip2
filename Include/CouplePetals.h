					 /*-----------------------------------------------------------------+
					  |						CouplePetals.h								|
					  |																	|
					  |	je vasi essayer de faire des petals de fleurs sur la 2eme scene	|
					  |				du couple, avec un chti doigt de physik				|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#ifndef _COUPLEPETALS_VIP2_H_
#define _COUPLEPETALS_VIP2_H_


class CoupleOnePetals3D	;	// from CouplePetals3D.h
class CoupleShadowP3D	;	// from CouplePetals3D.h

//////////////////////////////////////////////
class CoupleOnePetals{

//------------------ Datas
private:

	Ufloat Mass			;
	Ufloat Vitesse[3]	;
	Ufloat Pos[3]		;
	Ufloat Accel[3]		;

	Ufloat Tappear			;
	Ufloat totalTappear		;
	Ufloat Treste			;
	Ufloat Tdisappear		;
	Ufloat totalTdisappear	;

//------------------ Fonctions
public:

	CoupleOnePetals()	;

	void Ini( Ufloat tapp, Ufloat tres, Ufloat tdisa, Ufloat pos[3], Ufloat vitesse[3] )	;
	void IniAccel()																			;
	void AddForce( Ufloat force[3] )														;
	void CalcPos( Ufloat laptime )															;

	bool	IsFinish()	{if( Tdisappear<=0.0f) return TRUE; else return FALSE;}
	Ufloat	GetAlpha()	;
	Ufloat* GetPos()	{return Pos;}
	Ufloat	GetMass()	{return Mass;}
};
//////////////////////////////////////////////


//////////////////////////////////////////////
// c'est un sphere dans lakelle sévie une direction et une puissance de vent !
class CoupleSphereVent{

//------------------ Datas
private:

	Ufloat outer2		;
	Ufloat inner2		;
	Ufloat vec[3]		;
	Ufloat pos[3]		;
	Ufloat puissance	;

public:

	CoupleSphereVent* suiv	;

//------------------ fonctions
public:

	CoupleSphereVent( Ufloat inner, Ufloat outer, Ufloat puissance, Ufloat dir[3], Ufloat pos[3] )	;

	void CalcInfluence( CoupleOnePetals* peta, U32 nb )		;	// va calculer la force ki va s'appliker sur chak pétals
	void CalcInfluence( CoupleOnePetals3D* peta, U32 nb )	;
};
//////////////////////////////////////////////



//////////////////////////////////////////////
class CouplePetals{

//------------------ Datas
private:

	TextureU3D	*partTex	;	// image de la particule de base
	MaterialU3D	*partMat	;	// material de la particule de base
	BitmapU3D	*partBmp	;	// bitmpa de la particule

	TextureU3D	*Tex3D	;	// texture pour les petals 3D
	MaterialU3D *Mat3D	;	// material pour les petals 3D

	TextureU3D	*TexShadow3D	;	// texture pour les shadow 3D
	MaterialU3D *MatShadow3D	;	// material pour les shadow 3D 

	CoupleOnePetals*	AllPetals	;
	CoupleOnePetals3D*	AllPetals3D	;
	CoupleShadowP3D*	AllShadows	;	// fake shadows des petals
	CoupleSphereVent*	AllVents	;

//------------------ Fonctions
private:

	void AffOneParti( Ufloat pos[3], Ufloat Size, Ufloat alpha )	;
	
public:

	CouplePetals( MondeU3D* monde )	;
	~CouplePetals()					;

	void IncAnim3D( Ufloat time, Ufloat laptime )	;
	void UpdateFrame( Ufloat time, Ufloat laptime )	;
};
//////////////////////////////////////////////

#endif
