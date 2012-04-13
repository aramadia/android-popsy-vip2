	   				 /*-------------------------------------------------------------------------+
					  |							Final.h											|
					  |																			|
					  |			la toute derniere scene de la demo poil au mal de dos 			|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#ifndef _FINAL_VIP2_H
#define _FINAL_VIP2_H

/////////////////////////////////////////
class FinalPart{

//--------------- Datas
private:

	static UScheduler*	Scheduler	;

	static MondeU3D*	Monde	;
	static MondeU3D*	Back	;

	static FinalSpline* Spline ;

	static BPeloche*	Bruit	;
	static FinalTexte*	Textes	;

	static MaterialU3D*	MatVolume	;

	static Ufloat	VolumeLightIntens		;
	static Ufloat	VolumeLightIncIntens	;
	static Ufloat	Flat3DIntens			;
	static Ufloat	Flat3DIncIntens			;
	static Ufloat	Flat3DClign				;
	static Ufloat	Flat3DIncClign			;
	static Ufloat	Cartoon3DIntens			;
	static Ufloat	Cartoon3DIncIntens		;

	static bool		FirstTime	;
	static bool		FlipScreen	;
	static Udouble	LapTime		;
	static Udouble	LastTime	;

//--------------- Fonbctions
private:
	
	static void			IniSynchros()																;
	static void WINAPI	SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)	;
	static void WINAPI	AffCartoon( Ufloat step, U32 user )											;
	static void WINAPI	AffVolumeLight( Ufloat step, U32 user )										;
	static void			IncVars()																	;

public:

	static void			PreLoad()								;
	static void WINAPI	Initialise(UScheduler *sche, U32 user)	;
	static void WINAPI	Release(UScheduler *sche, U32 user)		;
	static void WINAPI	MainLoop(UScheduler *sche, U32 user)	;

	static void SetFlip( bool b )	{FlipScreen=b;}
};
/////////////////////////////////////////

#endif
