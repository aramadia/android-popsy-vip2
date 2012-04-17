/*-----------------------------------------------------+
 |                       VestU3D.h                      |
 |                                                      |
 |    pleins de petites macros bien utiles pour des     |
 |                      calculs 3D                      |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _VECTU3D_U3D3_H_
#define _VECTU3D_U3D3_H_

#include <math.h>
#include "Types.h"

//================================

#define VEC3_OPRVVV(A,B,OP,C)   {   (A)[0]=(B)[0] OP (C)[0];\
                                    (A)[1]=(B)[1] OP (C)[1];\
                                    (A)[2]=(B)[2] OP (C)[2];    }

#define VEC4_OPRVVV(A,B,OP,C)   {   (A)[0]=(B)[0] OP (C)[0];\
                                    (A)[1]=(B)[1] OP (C)[1];\
                                    (A)[2]=(B)[2] OP (C)[2];\
                                    (A)[3]=(B)[3] OP (C)[3];    }

#define VEC3_OPRVVS(A,B,OP,C)   {   (A)[0]=(B)[0] OP (C);\
                                    (A)[1]=(B)[1] OP (C);\
                                    (A)[2]=(B)[2] OP (C);   }

#define VEC4_OPRVVS(A,B,OP,C)   {   (A)[0]=(B)[0] OP (C);\
                                    (A)[1]=(B)[1] OP (C);\
                                    (A)[2]=(B)[2] OP (C);\
                                    (A)[3]=(B)[3] OP (C);   }

//================================

inline void vec3_div(Ufloat* c, const Ufloat* b, const Ufloat a) {
    VEC3_OPRVVS(c,b,/,a);
}

inline void vec4_div(Ufloat* c, const Ufloat* b, const Ufloat a) {
    VEC4_OPRVVS(c,b,/,a);
}

//================================

inline void vec3_mul(Ufloat* c, const Ufloat* b, const Ufloat* a) {
    VEC3_OPRVVV(c,b,*,a);
}
inline void vec3_mul(Ufloat* c, const Ufloat* b, Ufloat a) {
    VEC3_OPRVVS(c,b,*,a);
}

inline void vec4_mul(Ufloat* c, const Ufloat* b, const Ufloat* a) {
    VEC4_OPRVVV(c,b,*,a);
}
inline void vec4_mul(Ufloat* c, const Ufloat* b, Ufloat a) {
    VEC4_OPRVVS(c,b,*,a);
}

//================================

inline void vec3_set(Ufloat* c, Ufloat x,Ufloat y,Ufloat z) {
    c[0] = x;
    c[1] = y;
    c[2] = z;
}
inline void vec4_set(Ufloat* c, Ufloat x,Ufloat y,Ufloat z,Ufloat w) {
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = w;
}


//================================
inline void vec3_eg(Ufloat* c, const Ufloat* a) {
    c[0] = a[0];
    c[1] = a[1];
    c[2] = a[2];
}
inline void vec4_eg(Ufloat* c, const Ufloat* a) {
    c[0] = a[0];
    c[1] = a[1];
    c[2] = a[2];
    c[3] = a[3];
}

//================================

inline void vec3_add(Ufloat* c, Ufloat* a, Ufloat* b) {
    VEC3_OPRVVV(c,a,+,b);
}

inline void vec4_add(Ufloat* c, Ufloat* a, Ufloat* b) {
    VEC4_OPRVVV(c,a,+,b);
}

//=================================

inline void vec3_sub(Ufloat* c, const Ufloat* a, const Ufloat* b) {
    VEC3_OPRVVV(c,a,-,b);
}

inline void vec4_sub(Ufloat* c, const Ufloat* a, const Ufloat* b) {
    VEC4_OPRVVV(c,a,-,b);
}

//=================================;
#define VEC3_XPRD(D,A,B)\
    D[0] = A[1] * B[2] - A[2] * B[1]; \
    D[1] = A[2] * B[0] - A[0] * B[2]; \
    D[2] = A[0] * B[1] - A[1] * B[0]; \
 
inline void vec3_xprd(Ufloat* d, const Ufloat* a, const Ufloat* b) {
    VEC3_XPRD(d,a,b);
}

//=================================


inline Ufloat vec3_dot(const Ufloat* a, const Ufloat* b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

//=================================

inline Ufloat vec3_sqrt(const Ufloat* a) {
    return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
}

//=================================

inline void vec3_neg(Ufloat* a) {
    a[0] = -a[0];
    a[1] = -a[1];
    a[2] = -a[2];
}

//=================================

inline void vec3_normalize(Ufloat* a) {
    Ufloat  mag = vec3_sqrt(a);

    if (mag!=0)  mag = (Ufloat)1. / mag;
    vec3_mul(a, a, mag);
}

//=================================

inline void vec3_FastNormalize(Ufloat* a) {
    /*  Ufloat  mag = OneOnSqrt( a[0]*a[0] + a[1]*a[1] + a[2]*a[2] );
        vec3_mul( a, a, mag );*/
    vec3_normalize(a);
}

//=================================

inline Ufloat vec3_dist(const Ufloat* a, const Ufloat* b) {
    Ufloat tmp[3];
    vec3_sub(tmp, b, a);
    return vec3_sqrt(tmp);
}

//=================================


inline Ufloat vec3_distFake(const Ufloat* a, const Ufloat* b) {
    Ufloat tmp[3];
    vec3_sub(tmp, b, a);
    return tmp[0]*tmp[0] + tmp[1]*tmp[1] + tmp[2]*tmp[2];
}

//=================================

inline void vec3_rebond(Ufloat* dest, Ufloat* Vnormal, Ufloat* VVec) {
    Ufloat tmp[3];

    vec3_mul(tmp, Vnormal, 2.f*vec3_dot(Vnormal, VVec));
    vec3_sub(dest, VVec, tmp);
}

//=================================
#endif
