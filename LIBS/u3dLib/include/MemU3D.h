/*-----------------------------------------------------+
 |                       MemU3D.h                       |
 |                                                      |
 |    Petite macros pour la gestion de la m�moire       |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _MEMU3D_U3D3_H_
#define _MEMU3D_U3D3_H_

#ifdef __APPLE__

#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#define GetMemU3D(dest,nb,type)\
{ (dest)=(type*)malloc(sizeof(type)*(nb));\
  if( ((dest)==NULL) && ((nb)!=0) ) FatalError("Ya plus de memoire mon loire!");}

#define TestPtr(ptr)\
{if( ptr==NULL ) FatalError("Ya plus de memoire mon loire!");}

#define FreeMemU3D(ptr) {if(ptr!=NULL) free(ptr); ptr=NULL;}

#endif
