    				 /*-----------------------------------------------------+
					  |				       PFustrum.h						|
					  |														|
					  |	 définition d'une pyramide de fustrum ( comme pour  |
					  |	   une pyramide de vision en fait avec tout se ke 	|
					  |					cela implique ).					|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/

#ifndef _PFUSTRUM_U3D3_H
#define _PFUSTRUM_U3D3_H

/////////////////////////////////////////
class PFustrum{

//---------- datas
protected:

	Ufloat pos[3]	;	// position du fustrum
	Ufloat targ[3]	;	// position de la target du fustrum
	Ufloat vue[3]	;	// vecteur Pos->Targ normalisé
	Ufloat roll		;
	Ufloat fov		;

	Mat3x4	Trans	;	// matrice de transformation
	Mat3x4	Project	;	// matrice de projection
	Mat3x4	Final	;	// matrice final

	Plan gauche,droite,haut,bas,proche,lointain	;

//---------- Fonctions
public:

	PFustrum()	;

	void	SetPos(Ufloat p[3])						{vec3_eg( pos, p );}
	void	SetPos(Ufloat x, Ufloat y, Ufloat z)	{vec3_set(pos, x, y, z);}
	Ufloat *GetPos()								{return &pos[0];}

	void	SetTarg(Ufloat t[3])					{vec3_eg( targ, t );}
	void	SetTarg(Ufloat x, Ufloat y, Ufloat z)	{vec3_set(targ, x, y, z);}
	Ufloat *GetTarg()								{return &targ[0];}

	Plan* GetPlanDroit()	{return &droite;}
	Plan* GetPlanGauche()	{return &gauche;}
	Plan* GetPlanBas()		{return &bas;}
	Plan* GetPlanHaut()		{return &haut;}
	Plan* GetPlanProche()	{return &proche;}
	Plan* GetPlanLointain()	{return &lointain;}

	void			UpdatePlans()			;
	void			UpdateTransMatrix()		;
	virtual void	UpdateProjectMatrix()	;

	void Recalc()	;	// reclacl tout pour le fustrum
};
/////////////////////////////////////////

#endif
