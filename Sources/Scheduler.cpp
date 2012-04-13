    				 /*-------------------------------------------------------------+
					  |				         Scheduler.cpp							|
					  |																|
					  |	c'est lui ki va réguler le lancement des différentes taches	|
					  |																|
					  | 	U2^PoPsy TeAm 2000										|
					  +-------------------------------------------------------------*/

#include "stdafx.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-------------------+
//									|    Struct UTask	|
//									+-------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
UTask::UTask()
{
	nom = ""		;
	procs = NULL	;
	suspendu = FALSE;
	mustDie = FALSE	;
	suiv = NULL		;
}
//----------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|    Class Scheduler	|
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
UScheduler::UScheduler()
{
	AllTask = NULL		;
	LastTask = NULL		;
	ActuaTask = NULL	;
}
//----------------------------------------------------------------------------------------------------------
UScheduler::~UScheduler()
{
	UTask	*tmp	;

	while( AllTask )
	{
		tmp = AllTask->suiv	;
		delete AllTask		;
		AllTask = tmp		;
	}
}
//----------------------------------------------------------------------------------------------------------
UTask *UScheduler::Find( LPCSTR nom )
{
	UTask *tmp	;

	tmp = AllTask	;
	while( tmp )
	{
		if( tmp->nom == nom )
			return tmp	;
		tmp = tmp->suiv	;
	}

	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::SetLast()
{
	UTask	*tmp	;

	if( AllTask )
	{
		tmp = AllTask						;
		while( tmp->suiv )	tmp = tmp->suiv	;
		LastTask = tmp						;
	}
	else
	{
		LastTask = NULL	;
	}
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::AddTask( LPCSTR nom, PROCUTASK	procs )
{
	UTask	*tmp	;

	if( LastTask )
	{
		tmp = new UTask			;
		tmp->procs = procs		;
		tmp->nom = nom			;

		LastTask->suiv = tmp	;
		LastTask = tmp			;
	}
	else
	{
		tmp = new UTask			;
		tmp->procs = procs		;
		tmp->nom = nom			;

		AllTask = tmp	;
//		ActuaTask = tmp	;
		LastTask = tmp	;
	}
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::SuspendTask( LPCSTR nom )
{
	UTask *tmp	;

	tmp = Find( nom )	;

	if( tmp ) tmp->suspendu = TRUE	;
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::SuspendAllTask()
{
	UTask	*tmp	;

	tmp = AllTask	;
	while( tmp )
	{
		tmp->suspendu = TRUE	;
		tmp = tmp->suiv			;
	}	
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::ReScheduleTask( LPCSTR nom )
{
	UTask *tmp	;

	tmp = Find( nom )	;

	if( tmp ) tmp->suspendu = FALSE	;
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::ReScheduleAllTask()
{
	UTask	*tmp	;

	tmp = AllTask	;
	while( tmp )
	{
		tmp->suspendu = FALSE	;
		tmp = tmp->suiv			;
	}	
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::KillActuaTask()
{
	if( !ActuaTask )	return	;

	ActuaTask->mustDie = TRUE	;
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::KillTask( LPCSTR nom )
{
#if 0
	UTask	*tmp	;

	tmp = AllTask	;
	while( tmp )
	{
		if( tmp->nom = nom )
		{
			tmp->mustDie = TRUE	;
			break		;
		}
		tmp = tmp->suiv	;
	}
#endif
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::KillAllDeadTask()
{
	UTask	*tmp,*preced=NULL	;
	bool	need = FALSE		;

	tmp = AllTask	;
	while( tmp )
	{
		if( tmp->mustDie )
		{
			if( preced )
			{
				preced->suiv = tmp->suiv	;
				delete tmp					;
				tmp = preced->suiv			;
			}
			else
			{
				AllTask = tmp->suiv	;
				delete tmp			;
				tmp = AllTask		;
			}
			need = TRUE	;
		}
		else
		{
			preced = tmp	;
			tmp = tmp->suiv	;
		}
	}

	if( need ) SetLast()	;
}
//----------------------------------------------------------------------------------------------------------
void UScheduler::Run()
{
	ActuaTask = AllTask	;
	while( ActuaTask )
	{
		if( !ActuaTask->suspendu )
			(*ActuaTask->procs)( this, 0 )	;

		ActuaTask = ActuaTask->suiv	;
	}

	KillAllDeadTask()	; // voit si il fo killer des taches mortes 
}
//----------------------------------------------------------------------------------------------------------
