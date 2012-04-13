    				 /*-----------------------------------------------------+
					  |				         ClockU3D.h						|
					  |														|
					  |	   Une classe ki permet d'avoir un timer un temps	|
					  |			 soit peu + precis ke le clock()			|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/

#ifndef _Clock_U3D3_H
#define _Clock_U3D3_H

#include <sys/time.h>
#include <sys/types.h>

class ClockU3D{

	static struct timeval start;
	static double extra_adj;

public:
	static double mp3_timer;

	static void		Initialise()			;
	static void		Reset();
	static void		IncTime( Udouble time )	{extra_adj+=time;}
	static double	GetTime()				;
};

#endif
