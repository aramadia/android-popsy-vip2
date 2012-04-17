/*-----------------------------------------------------+
 |                       M44u3d.h                       |
 |                                                      |
 |     Les matrice 4x4 vont etre tres utiles pour       |
 |       les anims et tout le reste d'ailleur           |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _M44U3D_U3D3_H_
#define _M44U3D_U3D3_H_

#include <string.h>
#include "Types.h"

void m44_identite(Ufloat m[4][4]);
void m44_multiply(Ufloat d[4][4],Ufloat s1[4][4],Ufloat s2[4][4]);
void m44_scale(Ufloat m[4][4], Ufloat x, Ufloat y, Ufloat z);
void m44_translate(Ufloat m[4][4], Ufloat x, Ufloat y, Ufloat z);
inline void m44_eg(Ufloat d[4][4], Ufloat s[4][4]) {
    memcpy(d,s,16*sizeof(Ufloat));
}

#endif
