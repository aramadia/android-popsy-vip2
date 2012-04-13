	   			 /*---------------------------------------------------------------------+
				  |							HSplineAff.h								|
				  |																		|
				  |	va afficher une gnolie spline avec des lignes toute antialiasées	|
				  |						et accélérées									|
				  |																		|
				  | 	U2^PoPsy TeAm 2000												|
				  +---------------------------------------------------------------------*/


#ifndef _HSPLINEAFF_VIP2_H
#define _HSPLINEAFF_VIP2_H



//////////////////////////////////////////
// une structure temporaire bien utile et ki permet pas mal de souplesse
struct HSplinaeAff_stock{

	Ufloat time		;
	Ufloat x,y,z	;
	Ufloat r,v,b,a	;

	HSplinaeAff_stock*	suiv	;

	HSplinaeAff_stock()	{time=0.0f;
						 x=0.0f;y=0.0f;z=0.0f;
						 r=1.0f;v=1.0f;b=1.0f;a=1.0f;
						 suiv=NULL;}
};
//////////////////////////////////////////




//////////////////////////////////////////
class HSplinaeAff{

//-------------------- Datas
private:

	HSplinaeAff_stock*	tempKeys	;	// on y stock les clés temporairement
	HSplinaeAff_stock*	actuaKey	;	// clé temporaire actuelle
	U32					nbKey		;	// nombre de clé

	SplineU3D*	spline	;

	U32		nbmaxLine		;	// nombre maxi de ligne
	Ufloat	Tpos			;	// position dans le temps
	Ufloat	Tmax			;	// temps maxi
	Ufloat	RVBAStart[4]	;	// couleur de départ de la courbe
	Ufloat	RVBAEnd[4]		;	// couleur d'arrivée

	bool Spline2d	;	// FALSE par defaut
	bool Ztest		;	// FALSE par defaut
	U32	 Transp		;	// MatTranspAddAlpha par defaut


//-------------------- fonctions
private:

	void KillTempKeys()	;

public:

	HSplinaeAff( bool spline2d=FALSE)	;
	~HSplinaeAff()						;

	void AddKey( Ufloat time )										;
	void SetKeyPos( Ufloat x, Ufloat y, Ufloat z=0.0f )				;
	void SetKeyColor( Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f )	;
	void Confirm( U32 nbMaxline )									;	// pré-calcul de la spline ( en donnat un nombre maxi de ligne pour afficher la spline )

	void SetZtest( bool b )												{Ztest=b;}
	void SetTransp( U32 tr )											{Transp=tr;}
	void SetStartColor( Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f )	{vec4_set(RVBAStart,r,v,b,a);}
	void SetEndColor( Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f )		{vec4_set(RVBAEnd,r,v,b,a);}
	void SetStartAlpha( Ufloat a )										{RVBAStart[3]=a;}
	void SetEndAlpha( Ufloat a )										{RVBAEnd[3]=a;}

	void GetLastPos( Ufloat dest[3] )	;	// renvoie dernier position calculée de la spline

	bool IsFinish() {if(Tpos>=Tmax) return TRUE; else return FALSE;}

	void IniAnim()					{spline->Ini();}
	void IncAnim( Ufloat laptime )	{spline->IncPos( laptime );}

	void Affiche( Ufloat laptime )	;
};
//////////////////////////////////////////


#endif
