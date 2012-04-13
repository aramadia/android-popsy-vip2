    				 /*-------------------------------------------------------------+
					  |				         Synchronisater.cpp						|
					  |																|
					  |	C'est lui ki va s'occuper de gérer les callback de synchros	|
					  |																|
					  | 	U2^PoPsy TeAm 2000										|
					  +-------------------------------------------------------------*/


#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------------+
//									|   Class SynchroCallback	|
//									+---------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
SynchroCallback::SynchroCallback( PROCSYNCCALLBACK procs, U32 type )
{
	Procs = procs	;
	Type = type		;
	Call = FALSE	;
	suiv = NULL		;
}
//----------------------------------------------------------------------------------------------------------
void SynchroCallback::Run( SynchroTick *sync, Ufloat rtime, U32 syncNum )
{
	if( Type == onecall )
	{
		if( !Call )
		{
			Procs( 	sync, rtime, syncNum, 0 )	;
			Call = TRUE							;
		}
	}
	else
	{
		Procs( 	sync, rtime, syncNum, 0 )	;
		Call = TRUE							;
	}
}
//----------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|   Class SynchroTick	|
//										+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
SynchroTick::SynchroTick( U32 number, Ufloat pos )
{
	AllCallback = NULL	;
	Id = number			;
	Tpos = pos			;
	suiv = NULL			;
}
//----------------------------------------------------------------------------------------------------------
SynchroTick::~SynchroTick()
{
	SynchroCallback	*tmp	;

	while( AllCallback )
	{
		tmp = AllCallback->suiv	;
		delete AllCallback		;
		AllCallback = tmp		;
	}
}
//----------------------------------------------------------------------------------------------------------
void SynchroTick::AddCalBack(  PROCSYNCCALLBACK pr, U32 type )
{
	SynchroCallback	*tmp	;
	
	tmp = new SynchroCallback( pr, type )	;
	tmp->suiv = AllCallback					;
	AllCallback = tmp						;
}
//----------------------------------------------------------------------------------------------------------
void SynchroTick::Run( Ufloat time )
{
	SynchroCallback	*tmp	;

	tmp = AllCallback	;
	while( tmp )
	{
		tmp->Run( this, time - Tpos, Id )	;
		tmp = tmp->suiv						;
	}
}
//----------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|   Class SynchroPhase	|
//										+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
SynchroPhase::SynchroPhase( LPCSTR n, Ufloat start, Ufloat end )
{
	nom = n	;

	TStart = start	;
	TEnd = end		;

	AllTick = NULL	;
}
//----------------------------------------------------------------------------------------------------------
SynchroPhase::~SynchroPhase()
{
	SynchroTick* tmp	;

	while( AllTick )
	{
		tmp = AllTick->suiv	;
		delete AllTick		;
		AllTick	= tmp		;
	}
}
//----------------------------------------------------------------------------------------------------------
SynchroTick* SynchroPhase::AddTick( U32 number, Ufloat tpos, PROCSYNCCALLBACK procs, U32 type )
{
	SynchroTick *tmp = new SynchroTick( number, tpos )	;
	
	tmp->AddCalBack( procs, type )	;
	tmp->suiv = AllTick				;
	AllTick = tmp					;

	return tmp	;
}
//----------------------------------------------------------------------------------------------------------
void SynchroPhase::Run( Ufloat time	)
{
	SynchroTick*	tmp						;
	Ufloat			tpos = time - TStart	;

	tmp = AllTick	;
	while( tmp )
	{
		if( tmp->Tpos <= tpos )
			tmp->Run( tpos )	;
		tmp = tmp->suiv	;
	}
}
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+---------------------------+
//										|   Class USynchronisater	|
//										+---------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
USynchronisater::USynchronisater()
{
	AllPhase = NULL	;
}
//----------------------------------------------------------------------------------------------------------
USynchronisater::~USynchronisater()
{
	SynchroPhase *tmp	;

	while( AllPhase )
	{
		tmp = AllPhase->suiv	;
		delete AllPhase			;
		AllPhase = tmp			;
	}
}
//----------------------------------------------------------------------------------------------------------
SynchroPhase* USynchronisater::AddPhase( LPCSTR nom, Ufloat start, Ufloat end  )
{
	SynchroPhase*	ret				;
	SynchroPhase*	parcour			;
	SynchroPhase*	preced = NULL	;


	ret = new SynchroPhase( nom, start, end )	;

	// Ajoute la phase, mais en triant par le temps de debut de la phase
	parcour = AllPhase	;
	while( parcour )
	{
		if( parcour->GetStartTime() > ret->GetStartTime() )
			break	;

		preced = parcour		;
		parcour = parcour->suiv	;
	}

	if( preced )
	{
		preced->suiv = ret	;
		ret->suiv = parcour	;
	}
	else
	{
		ret->suiv = AllPhase	;
		AllPhase = ret			;
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
// TODO : optimiser en virant les phases ki sont terminées !
void USynchronisater::Run()
{
	SynchroPhase*	tmp												;
	Ufloat			actuaTime = ClockU3D::GetTime() - LaunchTime	;

	tmp = AllPhase	;
	while( tmp )
	{
		if( tmp->GetStartTime() <= actuaTime )
		{
			if( tmp->GetEndTime() >= actuaTime ) {
				tmp->Run( actuaTime )	;
			}
		}
		else
		{
			break	;
		}

		tmp = tmp->suiv	;
	}
}
//----------------------------------------------------------------------------------------------------------
