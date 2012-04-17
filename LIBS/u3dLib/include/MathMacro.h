/*-------------------------------------------------------------+
 |                       MathMacro.h                            |
 |                                                              |
 |  des macros en ASM ou non pour aller plus vite sur certaine  |
 |                  opérations mathématiques                    |
 |                                                              |
 |  U2^PoPsy TeAm 2000                                      |
 +-------------------------------------------------------------*/

#ifndef _MathMacros_U3D3_H_
#define _MathMacros_U3D3_H_


#include <math.h>

/////////////////////////////////////////
// une methode pour approximer le 1/sqrt(x) à environ 1-2 % d'erreur
// le code a été inspiré par la doc de Skal ( merci mec ! )

extern float OneOnSqrt_Seed[2048];
extern void Build_OneOnSqrtTab();

inline float OneOnSqrt(float base) {
#if 1
    return 1.0f / sqrt(base);
#else
    Ufloat const2 = 2.0f;

    __asm {

        fld base                            // st = base
        mov eax, base

        fld const2                          // st = 2.0f | base
        shr eax, 20
        and eax, 0x07fffffff
        fld [OneOnSqrt_Seed+4*eax]  // st = x | 2 | base

        // on  va faire 3 iterations

        // iteration 1
        fld st              // st = x | x | 2 | base
        fmul st, st(3)      // st = base*x | x | 2 | base
        fmul st, st(1)      // st = base*x*x | x | 2 | base
        fsubr st, st(2)     // st = 2 - base*x*x | x | 2 | base
        fmulp st(1), st     // st = x + dx | 2 | base */

        // iteration 2
        fld st              // st = x | x | 2 | base
        fmul st, st(3)      // st = base*x | x | 2 | base
        fmul st, st(1)      // st = base*x*x | x | 2 | base
        fsubr st, st(2)     // st = 2 - base*x*x | x | 2 | base
        fmulp st(1), st     // st = x + dx | 2 | base */

        // iteration 3 ( la derniere hihihih )
        fxch st(2)          // st = base | 2 | x
        fmul st, st(2)      // st = base*x | 2 | x
        fmul st, st(2)      // st = base*x*x | 2 | x
        fsubp st(1), st     // st = 2 - base*x*x | x
        fmulp st(1), st     // st = x + dx

        fstp base
    }

    return base;
#endif
}
/////////////////////////////////////////


#endif
