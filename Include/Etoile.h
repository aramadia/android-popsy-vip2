	   				 /*-------------------------------------------------------------------------+
					  |							Etoile.h										|
					  |																			|
					  |	Cette partie s'appelle etoile non pas paskeu ya des myriades d'objets 	|
					  |	Celestes, mais plutot paskeu l'objet ki se deforme au 1er plan ressemble|
					  |			a une salle etoile de mer toute moisie vala.					|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#ifndef _ETOILE_VIP2_H
#define _ETOILE_VIP2_H

/////////////////////////////////////////
class EtoilePart{

//--------------- Datas
private:

	static UScheduler*	Scheduler	;

	static MondeU3D*		Monde	;

	static ObjDeformation*	EtoileDef	;
	static EtoileScale*		Scale		;
	static EtoileGreets*	Greets		;

	static UImage* Fondu ;

	static U32 TextePos	;	// position dans les textes des greets

	static bool	   FirstTime	;
	static Udouble LapTime		;
	static Udouble LastTime		;

//--------------- Fonctions
private:

	static void			IniSynchros()																;
	static void WINAPI	SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)	;
	static void	WINAPI	AffScale( Ufloat step, U32 user )											;

public:

	static void			PreLoad()								;
	static void WINAPI	Initialise(UScheduler *sche, U32 user)	;
	static void WINAPI	Release(UScheduler *sche, U32 user)	;
	static void WINAPI	MainLoop(UScheduler *sche, U32 user)	;

	static void NextCamera()	;

};
/////////////////////////////////////////

#endif

