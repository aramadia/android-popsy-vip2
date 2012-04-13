					 /*-----------------------------------------------------------------+
					  |							Couple.h								|
					  |																	|
					  |	 normalement une magnifik scene avec un couple sous la pluie	|
					  |			battante ki s'envol vers un monde meilleur				|
					  |					( snif c'est émouvant... )						|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/


#ifndef _COUPLE_VIP2_H_
#define _COUPLE_VIP2_H_


//////////////////////////////////////////////
// défintion de la class statik
class CouplePart{

//------------------ Datas
private:

	static UScheduler	*Scheduler	;

	static MondeU3D*	Monde		;
	static MondeU3D*	Monde1		;
	static MondeU3D*	ActuaMonde	;
	static bool			ScenePluie	;	// si on est pas dans la scene de la pluie, on est dans celle des petales de roses !
	static U32			ActuaCamSc1	;	// info sur la camera pour savoir ki sera la prochaine activée
	static U32			ActuaCamSc2	;

	static CouplePetals*	Petals		;
	static CoupleRPluie*	Bruine		;
	static PluieL*			pluieLigne	;
	static PluieP*			pluiePoly	;

	static CoupleTextes*	Textes	;
	static UImage*			Flash	;
	static UImage*			Fondu	;

	static bool	   FirstTime	;
	static Udouble LapTime		;
	static Udouble LastTime		;

//------------------ Fonctions
private:

	static void			IniSynchros()																;
	static void WINAPI	SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)	;
	static void			NextCamScene1()																;
	static void			NextCamScene2()																;

public:

	static void			PreLoad()								;
	static void WINAPI	Initialise(UScheduler *sche, U32 user)	;
	static void WINAPI	Release(UScheduler *sche, U32 user)		;
	static void WINAPI	MainLoop(UScheduler *sche, U32 user)	;

};
//////////////////////////////////////////////

#endif

