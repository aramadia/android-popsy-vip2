/*-----------------------------------------------------+
 |                       BSpline.cpp                    |
 |                                                      |
 |   c'est a peut pres comme spline.h a ceci prés       |
 |   ke se sont des Bezier Spline ki sont utilisées ici |
 | ( la spline ne passe pas forcément par tout les pts  |
 |                      de controls ).                  |
 |                                                      |
 |  U2^PoPsy TeAm 2000                              |
 +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class BsplineU3D |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
// donne le nombre de control points et la dimension de la spline
BSplineU3D::BSplineU3D(U32 nbC, U32 nbD) {
    if (nbC <4) nbC = 4;
    nbControls = nbC;

    if (nbD <1) nbD = 1;
    nbDimension = nbD;

    tab = new Ufloat[ nbC*nbD ];
}
//----------------------------------------------------------------------------------------------------------------------------------------
BSplineU3D::~BSplineU3D() {
    delete [] tab;
}
//----------------------------------------------------------------------------------------------------------------------------------------
Ufloat* BSplineU3D::GetControlPt(U32 nb) {
    if (nb >= nbControls) return NULL;
    return &tab[nbDimension*nb];
}
//----------------------------------------------------------------------------------------------------------------------------------------
// position dans la spline ( 0 < pos < 1 )
void BSplineU3D::GetVal(Ufloat* dest, Ufloat actua) {
    int pos = actua * (nbControls-4);
    Ufloat* control = GetControlPt(pos);

    Ufloat t  = actua*(nbControls-4) - pos;
    Ufloat t2 = t*t;
    Ufloat t3 = t2*t;

    Ufloat c1 = (0.5f * ((-1.0f * t3) + (2.0f * t2) + (-1.0f * t)));
    Ufloat c2 = (0.5f * ((3.0f * t3) + (-5.0f * t2) + (0.0f * t) + 2.0f));
    Ufloat c3 = (0.5f * ((-3.0f * t3) + (4.0f * t2) + (1.0f * t)));
    Ufloat c4 = (0.5f * ((1.0f * t3) + (-1.0f * t2) + (0.0f * t)));

    for (U32 b=0; b<nbDimension; b++) {
        dest[b] = c1*control[0+b] +
                  c2*control[nbDimension*1+b] +
                  c3*control[nbDimension*2+b] +
                  c4*control[nbDimension*3+b];
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
