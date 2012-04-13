	   				 /*---------------------------------------------------------------------+
					  |				         Neurone.h										|
					  |																		|
					  |	une jolie partie ou la camera se ballade dans un champ de "neurones"|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#ifndef _NEURONE_VIP2_H
#define _NEURONE_VIP2_H

/////////////////////////////////////////
class NeuronePart{

//--------------- Datas
private:

	static UScheduler*	Scheduler	;

	static MondeU3D*		Monde		;
	static NeuroBackground*	Background	;
	static NeuroLights*		Lights		;
	static NeuroLines*		Lines		;
	static NeuroTxt*		Textes		;

	static UImage*		FlashBlanc	;
	static UImage*		FondNoir	;
	static UImage*		RotoTRuc	;
	static XGrezille*	Grezille	;

	static Ufloat CamTrembleIntensX	;	// intensité du tremblement de l camera en x
	static Ufloat CamTrembleIntensY	;	// intensité du tremblement de l camera en y

	static Viewport SCviewport	;

	static bool	   FirstTime	;
	static Udouble LapTime		;
	static Udouble LastTime		;

//--------------- Fonctions
private:

	static void			IniSynchros()																	;
	static void SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)	;
	static void CameraTremblement( Camera *cam, Ufloat inc, U32 user )							;
	static void			AffFlashBlanc( Ufloat laptime )													;

public:

	static void			PreLoad()								;	// load des trucs avant la commencement de la séquence
	static void Initialise(UScheduler *sche, U32 user)	;
	static void Release(UScheduler *sche, U32 user)		;
	static void MainLoop(UScheduler *sche, U32 user)	;

};
/////////////////////////////////////////

#endif
