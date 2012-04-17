/*-----------------------------------------------------+
 |                       Emessages.cpp                  |
 |                                                      |
 |   Ici je gère l'envoie de messages pour le debuging  |
 |          ou érreur fatal ou truc dans le genre okay  |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/


#include <stdarg.h>
#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
//                              +---------------------+
//                              |    Les Fonctions    |
//                              +---------------------+
//----------------------------------------------------------------------------------------------------------

void OutputDebugString(const char* str) {
    printf("%s\n", str);
}

//----------------------------------------------------------------------------------------------------------
void FatalError(const char* text) {
    fprintf(stderr, "Fatal error: %s\n", text);
    exit(-1);
}
//----------------------------------------------------------------------------------------------------------
void DebugOutput(LPCSTR text) {
    OutputDebugString(text);
}
//----------------------------------------------------------------------------------------------------------
void ErrorMess(const char* format, ...) {
    va_list args;
    char    buffer[255];

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    fprintf(stderr, "Error: %s\n", buffer);
}
//----------------------------------------------------------------------------------------------------------
void DebugErrorMess(const char* format, ...) {
    va_list args;
    char    buffer[255];

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    OutputDebugString(buffer);
}
//----------------------------------------------------------------------------------------------------------
