    				 /*-----------------------------------------------------+
					  |				        Utils.h							|
					  |														|
					  |		des pitites fonctions utiles dans divers		|
					  |						cas de figures					|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/

#ifndef _UTILS_VIP2_H
#define _UTILS_VIP2_H

//=================================

Ufloat inline Random1()
{
	return (Ufloat) rand() / RAND_MAX	;
}

//=================================

#define DeleteBlinde(ptr){\
if(ptr)\
{\
	delete ptr;\
	ptr = NULL;\
}\
}

//=================================

Ufloat inline GetUniXpos( Ufloat x )	{return x / DEFAULT_SCREEN_X;}
Ufloat inline GetUniYpos( Ufloat y )	{return y / DEFAULT_SCREEN_Y;}

//=================================

extern double	TimePreced	;
extern U32		TickNum		;
extern float	m_fps		;

void inline AffFps()
{
	double	temps	;
	double	ti		;

	TickNum ++							;
	ti = ClockU3D::GetTime()			;
	temps = (double)( ti - TimePreced )	;
	if( temps > .5 )
	{
		m_fps = (float)TickNum / temps	;
		TimePreced = ti					;
		TickNum = 0						;
	}
	glColor3f(1.0F, 1.0F, 1.0F)								;
	TextOpenGL( 0, Yres-20, 20, "%3.2f FPs", m_fps )		;
}

//=================================

void GrabScreen( U32 x, U32 y, U32 num )	;


#endif
