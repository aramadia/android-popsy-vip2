    				 /*-----------------------------------------------------+
					  |				         light.h						|
					  |														|
					  |	 ralalal la gestion des lumières dans un moteur 3D  |
					  |	   est qq chose d'important car ca donne tout de 	|
					  |				suite bocoup mieux avec que sans		|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/


#ifndef _Light_U3D3_H
#define _Light_U3D3_H

/////////////////////////////////////////
// la classe abstraite pour les lights du moteur
class CLightU3D{

//---------- Datas
protected:

	CString Nom		;
	CLightU3D* suiv	;

//---------- Fonctions
public:

	CLightU3D( LPCSTR nom )	{Nom=nom;suiv=NULL;}
	virtual ~CLightU3D()=0	;

			LPCSTR	GetNom()	{return (LPCSTR)Nom;}
	virtual LPCSTR	GetType()=0	;	// renvoie le type de la light

	virtual bool IsAnimated()=0	;	// est-ce que la lumière est animée ?

	virtual void IniAnim()=0					;
	virtual void IncAnim( Ufloat laptime )=0	;

	virtual void CalcGouraudStaticLighting( Ufloat *dest, ObjetU3D *obj  )=0	;
	virtual void CalcLightingRealTime( ObjetU3D *obj )=0						;	// va calculer le lighting pour l'objet 
};
/////////////////////////////////////////



//====================

#define TYPE_LIGHT_OMNI	0x1
#define TYPE_LIGHT_SPOT	0x2

class LightU3D{

	CString Nom		;
	U32		Type	;

	Ufloat Pos[3]	;

	Ufloat RVB[3]	;
	Ufloat Intensity;

	bool   Attenuation	;	// est-ce que l'attenuation est gérée ?
	Ufloat Inner		;
	Ufloat Outer		;

	//---------- donnée relative au spot !
	Ufloat	Targ[3]	;
	Ufloat	Vec[3]	;
	Ufloat	Hotspot	;
	Ufloat	Falloff	;

	//----------- Animation
	SplineU3D	*Spos		;	// mouvement dans le temps
	SplineU3D	*Starg		;	// present si c'est un spot
	SplineU3D	*Scolor		;	// Spline des couleurs de le light

public:

	LightU3D()	;
	~LightU3D()	;

	bool Serialize( CArchive &ar )	;

	void SetType(U32 type)		{Type=type;}
	void SetPos(Ufloat p[3])	{vec3_eg(Pos,p);}
	void SetTarg( Ufloat p[3])	{vec3_eg(Targ,p);}
	void SetRVB(Ufloat p[3])	{vec3_eg(RVB,p);}
	void SetIntensity(Ufloat i)	{Intensity = i;}
	void SetAttenuation(bool b)	{Attenuation=b;}
	void SetHotspot(Ufloat h)	{Hotspot=h;}
	void SetFalloff(Ufloat f)	{Falloff=f;}

	operator LPCSTR()				{return (LPCSTR)Nom;}
	LPCSTR	GetNom()				{return (LPCSTR)Nom;}
	U32		GetType()				{return Type;}
	void	GetPos(Ufloat p[3])		{vec3_eg(p,Pos);}
	void	GetRVB(Ufloat dest[3])	{vec3_eg( dest, RVB );}
	Ufloat	GetIntensity()			{return Intensity;}
	Ufloat	GetInner()				{return Inner;}
	Ufloat	GetOuter()				{return Outer;}
	void	GetTarg(Ufloat t[3])	{vec3_eg(t,Targ);}
	Ufloat	GetHotspot()			{return Hotspot;}
	Ufloat	GetFalloff()			{return Falloff;}

	bool IsAnimated()	;	// est-ce que la lumière est animée ?

	void Normalize()	;		// remet les données à jour comme il fo

	void CalcGouraudStaticLighting( Ufloat *dest, ObjetU3D *obj  )	;
	void CalcLightingRealTime( ObjetU3D *obj )						;	// va calculer le lighting pour l'objet 

	void IniAnim()					;
	void IncAnim( Ufloat laptime )	;

};





#ifdef _LMAPS
//======================
// en gros 1 texel de la lightmap 
struct Lumel{

	Ufloat	RVB[3];

	Lumel(){ vec3_set( RVB, 0.f, 0.f, 0.f ); }
	void Add( Ufloat rvb[3] ){vec3_add( RVB, RVB, rvb );}
	void Conv( U8 dest[3] );

};



//======================
// va definir la mystérieuse et troublante lightmap

class Lightmap{

	U32		largeur,hauteur;

	Lumel	*data;

public:

	Lightmap *suiv;

	Lightmap( U32 largeur, U32 hauteur );
	~Lightmap();

	Ufloat GetHauteur(){return hauteur;}
	Ufloat GetLargeur(){return largeur;}

	void inline Add( U32 largeur, U32 hauteur, Ufloat rvb[3] );

	// va lisser la lightmap ( util can ya des ombres portées dessus ! )
	void Smooth();

	// copie lightmap dans une surface RVB normale
	// ATTENTION dest doit etre alloué comem il fo 
	void ToSurface( U8 *dest );
};
#endif	// _LMAPS

#endif
