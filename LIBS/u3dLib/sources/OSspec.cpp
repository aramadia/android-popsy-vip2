    		 /*-----------------------------------------------------+
			  |				          OSspec.cpp				    |
			  |														|
			  |tout les trucs sp�cifiques a un OS quoa sont definit |
			  |					ici poil au (....)					|
			  |											            |
			  | 	U2^PoPsy TeAm 1999								|
			  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
//								+---------------+
//								| LEs Fonctions |
//								+---------------+
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//								des fonctions locales
//----------------------------------------------------------------------------------------------------------
#if 0
void WindowsLife(  HWND hwnd )
{
	MSG msg;

	if ( PeekMessage( &msg, hwnd, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage ( &msg ); // Pour pouvoir traiter les messages pendant que l'animation se joue.
		DispatchMessage (&msg);
	}
}
#endif
//----------------------------------------------------------------------------------------------------------