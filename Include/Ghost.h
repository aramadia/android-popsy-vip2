					 /*-----------------------------------------------------------------+
					  |							Ghost.h									|
					  |																	|
					  |	 C'est scene s'appelle ghost paskeu tout simplement on s'est	|
					  | inspiré de de la gonzesse de ghost in the shell remplit de cable|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#ifndef _GHOST_VIP2_H_
#define _GHOST_VIP2_H_


//////////////////////////////////////////////
// défintion de la class statik
class GhostPart{

//------------------ Datas
private:

	static UScheduler	*Scheduler	;

	static MondeU3D*		Monde	;
	static GhostEclaire*	Eclairs	;

	static GhostVPort* Viewsp ;

	static Ghost2D* Effet2D	;

	static bool	   FirstTime	;
	static Udouble LapTime		;
	static Udouble LastTime		;

	static bool	FlipScreen	;
	
//------------------ Fonctions
private:

	static void	IniSynchros()																			;
	static void WINAPI SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)	;
	static void WINAPI AffEclairs(Ufloat step, U32 user)												;

public:

	static void	SetFlip( bool b )	{FlipScreen=b;}

	static void			PreLoad()								;
	static void WINAPI	Initialise(UScheduler *sche, U32 user)	;
	static void WINAPI	Release(UScheduler *sche, U32 user)	;
	static void WINAPI	MainLoop(UScheduler *sche, U32 user)	;

};
//////////////////////////////////////////////


#endif
