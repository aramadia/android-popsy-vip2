/*-----------------------------------------------------+
 |                       Emessages.h                    |
 |                                                      |
 |   Ici je gère l'envoie de messages pour le debuging  |
 |          ou érreur fatal ou truc dans le genre okay  |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _emess_U3D3_H_
#define _emess_U3D3_H_

void FatalError(const char* text);
void DebugOutput(const char* text);
void ErrorMess(const char* format, ...);
void DebugErrorMess(const char* format, ...);

#endif
