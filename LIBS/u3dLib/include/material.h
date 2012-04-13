    			 /*-----------------------------------------------------+
				  |				         material.h						|
				  |														|
				  |	 et op une chtite définition de material pour les   |
				  |					polygones 3D						|
				  |														|
				  | 	U2^PoPsy TeAm 1999								|
				  +-----------------------------------------------------*/

#ifndef _material_U3D3_H
#define _material_U3D3_H

#include <Vectu3d.h>

//==================
// definition d'un material

	//---------------------- Flag de shading
	#define MatShadingRien		0x0
	#define MatShadingFlat		0x2
	#define MatShadingGouraud	0x3
	#define MatShadingPhong		0x4

	//---------------------- Flag de transparence
	#define MatTranspRien		0x0
	#define MatTranspTrou		0x1
	#define MatTranspAdd		0x3
	#define MatTranspMul		0x4
	#define MatTranspAddAlpha	0x5
	#define MatTranspMulAlpha	0x6
	#define MatTranspAlpha		0x7
	#define MatTranspIncrust	0x8

class MaterialU3D{
	char *nom	;

	Ufloat RVBA[4]	;		// couleur material

	//------------------- Infos textures
	TextureU3D	*Tex1							;
	Ufloat		Xstart1,Ystart1,Xdelta1,Ydelta1	;	// localisation dans la map dans la texture	( valeur unitaire )
	U32			Transparence1					;
	TextureU3D	*Tex2							;
	Ufloat		Xstart2,Ystart2,Xdelta2,Ydelta2	;	// localisation dans la map  dans la texture ( valeur unitaire )
	U32			Transparence2					;

	bool	Side2			;
	U32		Shading			;
	bool	SpecularShading	;	// est-ce ke l'on permet le specular sur ce material

	void	*MatPipe	;	// pointe sur une instance de la classe MatPipeline ( hack code pour un tri par texture + rapide! )

public:

	MaterialU3D(char *n)	;
	~MaterialU3D()			;

	MaterialU3D& operator=( MaterialU3D& mat )	;

	void Side( bool s=TRUE)	{Side2=s;}
	void Confirm()			;					// remet a jour MatPipe !!!!!!!! ATTENTION doit etre fait apres une MODIF de texture ou de shading !!!

	void SetNom( char *n )										{nom=strdup(n);}
	void SetShading( U32 shad )									{Shading=shad;}
	void SetSpecular( bool b )									{SpecularShading=b;}
	void SetRouge( Ufloat d )									{RVBA[0]=d;}
	void SetVert( Ufloat d )									{RVBA[1]=d;}
	void SetBleu( Ufloat d )									{RVBA[2]=d;}
	void SetAlpha( Ufloat d )									{RVBA[3]=d;}
	void SetTransp( U32 tr )									;
	void SetTranspTex2( U32 tr )								;
	void SetTexture1( TextureU3D *tex )							;
	void SetTexture2( TextureU3D *tex )							;
	void SetTextures( TextureU3D *tex1, TextureU3D *tex2 )		;
	void SetColor( Ufloat col[4] )								{vec4_eg( RVBA, col);}
	void SetColor( Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f ){vec4_set( RVBA, r, v, b, a);}
	void SetColorFromTXM(U32 r, U32 v, U32 b, U32 a )			;
	void SetTex1PosFromTXM( Ufloat u, Ufloat v )				;
	void SetTex2PosFromTXM( Ufloat u, Ufloat v )				;
	void SetTex1DeltaFromTXM( Ufloat u, Ufloat v )				;
	void SetTex2DeltaFromTXM( Ufloat u, Ufloat v )				;
	void SetTex1Pos( Ufloat u, Ufloat v, Ufloat du, Ufloat dv )	{Xstart1=u;Ystart1=v;Xdelta1=du;Ydelta1=dv;}
	void SetTex2Pos( Ufloat u, Ufloat v, Ufloat du, Ufloat dv )	{Xstart2=u;Ystart2=v;Xdelta2=du;Ydelta2=dv;}

	bool		GetSide()														{return Side2;}
	U32			GetShading()													{return Shading;}
	LPCSTR		GetNom()														{return (LPCSTR)nom;}
	TextureU3D*	GetTex1()														{return Tex1;}
	U32			GetTrTex1()														{return Transparence1;}
	void		GetTex1Pos( Ufloat &sx, Ufloat &sy, Ufloat &dx, Ufloat &dy )	{sx=Xstart1;sy=Ystart1;dx=Xdelta1;dy=Ydelta1;}
	TextureU3D*	GetTex2()														{return Tex2;}
	U32			GetTrTex2()														{return Transparence2;}
	void		GetTex2Pos( Ufloat &sx, Ufloat &sy, Ufloat &dx, Ufloat &dy )	{sx=Xstart2;sy=Ystart2;dx=Xdelta2;dy=Ydelta2;}
	Ufloat		GetRouge()														{return RVBA[0];}
	Ufloat		GetVert()														{return RVBA[1];}
	Ufloat		GetBleu()														{return RVBA[2];}
	Ufloat		GetAlpha()														{return RVBA[3];}
	Ufloat*		GetCoul()														{return RVBA;}
	void*		GetPipe()														{if(!MatPipe) Confirm(); return MatPipe;}

	bool	IsSpecular()	{return SpecularShading;}

	bool Cmp(LPCSTR n)	;

	void Serialize( CArchive &ar )	;
};

#endif
