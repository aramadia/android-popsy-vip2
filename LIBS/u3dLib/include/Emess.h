    				 /*-----------------------------------------------------+
					  |				         Emessages.h				    |
					  |														|
					  |	 Ici je g�re l'envoie de messages pour le debuging  |
					  |			ou �rreur fatal ou truc dans le genre okay  |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifndef _emess_U3D3_H_
#define _emess_U3D3_H_

void FatalError(char *text)						;
void DebugOutput(char *text)						;
void ErrorMess( char* format, ...)			;
void DebugErrorMess( char* format, ... )	;

#endif
