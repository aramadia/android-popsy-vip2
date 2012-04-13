    	 /*-----------------------------------------------------+
		  |				         MemU3D.h					    |
		  |														|
		  |	  Petite macros pour la gestion de la mémoire	    |
		  |														|	
		  | 	U2^PoPsy TeAm 1999								|	
		  +-----------------------------------------------------*/

#ifndef _MEMU3D_U3D3_H_
#define _MEMU3D_U3D3_H_

#include <malloc.h>

#define GetMemU3D(dest,nb,type)\
{ (dest)=(type*)malloc(sizeof(type)*(nb));\
  if( ((dest)==NULL) && ((nb)!=0) ) FatalError("Ya plus de mémoire mon loire!");}

#define TestPtr(ptr)\
{if( ptr==NULL ) FatalError("Ya plus de mémoire mon loire!");}

#define FreeMemU3D(ptr) {if(ptr!=NULL) free(ptr); ptr=NULL;}

#endif
