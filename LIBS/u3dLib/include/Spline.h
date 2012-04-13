    				 /*-----------------------------------------------------+
					  |				         Spline.h						|
					  |														|
					  |	   Bon ici on calcul les courbes pour avoir des		|
					  |			 mouvements bien tip top fluides			|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/


#ifndef _Spline_U3D3_H
#define _Spline_U3D3_H

#include <linuxcompat.h>

class	ObjetU3D;	// definition ultérieur

//==================
// definition d'une référence de morphing

class MorphRefU3D{

	CString	nom		;
	Ufloat	*xyz	;	// positions dans l'espace
	U32		nbp		;

public:

	MorphRefU3D	*suiv	;

	MorphRefU3D( LPCSTR nom, U32 nb )	;
	~MorphRefU3D()						;

	LPCSTR GetNom()		{return (LPCSTR)nom;}
	float	*GetPtr()	{return xyz;}


};


//==================

struct SplineKey{
	Ufloat v		;	
	Ufloat ds,dd	;

	SplineKey()	{v=0;ds=0;dd=0;}
};

//==================

struct SplineRotKey{
	Quat	q				;
	Quat	quad_a,quad_b	;
};


//==================

struct SplineMorphKey{

	MorphRefU3D	*morph	;
	Ufloat		*ds,*dd	;

	~SplineMorphKey()	{delete [] ds;delete [] dd;}
};

//==================

struct SplineInfoKey{
	Ufloat		dt	;	// (t2 - t1)
	float	frame	;	// position actuel !
	SplineInfoKey()	{dt=0;frame=0.0f;}
};



//==================
// definition d'une slpine du moteur

class SplineU3D{

	U32		nbKey			;	// nombre de clé
	U32		actualNumKey	;	// numero clé actuel
	U32		nbDim			;	// nombre de dimension

	SplineInfoKey	*inf		;	// infos relatives à toutes les clés
	SplineKey		*Keys		;	// les clés
	SplineInfoKey	*actualInf	;
	SplineKey		*actualKey	;


	void SetDerivKey(SplineKey *skp, SplineKey *sk, SplineKey *skn,
				KeyHeaderU3D *hp,KeyHeaderU3D *h,KeyHeaderU3D *hn)	;

public:

	SplineU3D( U32 nbk, U32 nbdim, Ufloat *key, KeyHeaderU3D *kinfos  )	;
	SplineU3D( PosKeyFileU3D *pos, U32 nbk )							;
	SplineU3D( UniKeyFileU3D *uni, U32 nbk )							;
	~SplineU3D()														;

	void Ini()				;	// remet la parcour de la spline au debut
	void SetPos( float p )	;	// met la courbe a une position donnée 
	void IncPos( float n )	;	// incrémente position dans la spline de n valeur

	void GetVal( Ufloat *dest )	;	// renvoie valeur actuelle sur la spline
};




//==================
// definition d'une slpine de rotation du moteur

class SplineRotU3D{

	U32		nbKey			;	// nombre de clé
	U32		actualNumKey	;	// numero clé actuel

	SplineInfoKey	*inf			;	// infos relatives à toutes les clés
	SplineRotKey	*Keys			;	// les clés
	SplineInfoKey	*actualInf		;
	SplineRotKey	*actualRotKey	;


	void CompAB(SplineRotKey *skp, SplineRotKey *sk, SplineRotKey *skn,
				KeyHeaderU3D *hp,KeyHeaderU3D *h,KeyHeaderU3D *hn)	;

public:
	
	SplineRotU3D( U32 nbk )						;
	SplineRotU3D( RotKeyFileU3D *rot, U32 nbk )	;
	~SplineRotU3D()								;

	void Ini()				;	// remet la parcour de la spline au debut
	void SetPos( float p )	;	// met la courbe a une position donnée 
	void IncPos( float n )	;	// incrémente position dans la spline de n valeur

	void GetVal(  Quat &q  )	;	// renvoie valeur actuelle sur la spline

};





//==================
// definition d'une slpine de morphing

class SplineMorphU3D{

	U32		nbKey			;	// nombre de clé
	U32		actualNumKey	;	// numero clé actuel
	U32		nbPosInMorph	;	// combien de coords pour l'objet en fait

	MorphRefU3D		*AllRef			;	// toutes les références de morphing
	SplineInfoKey	*inf			;	// infos relatives à toutes les clés
	SplineMorphKey	*Keys			;	// les clés
	SplineInfoKey	*actualInf		;
	SplineMorphKey	*actualMorphKey	;

	MorphRefU3D *AddOrCreateRef( MorphKeyFileU3D *morph )	;
	void		 SetDerivKey(SplineMorphKey *skp, SplineMorphKey *sk, SplineMorphKey *skn,
							 KeyHeaderU3D *hp,KeyHeaderU3D *h,KeyHeaderU3D *hn)	;

public:

	SplineMorphU3D( MorphKeyFileU3D *morph, U32 nbk )	;
	~SplineMorphU3D()									;

	void Ini()				;
	void SetPos( float p )	;
	void IncPos( float n )	;

	void GetVal(  ObjetU3D *obj  )	;	// renvoie valeur actuelle sur la spline ( ici remplt le tableau de coord de l'objet ! )

};


#endif
