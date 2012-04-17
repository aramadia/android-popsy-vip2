/*---------------------------------------------------------------------+
 |                          BSplineAff.cpp                              |
 |                                                                      |
 |  va afficher une gnolie spline avec des lignes toute antialiasées    |
 |      et accélérées ( ca ressemble a HSplineAFF n'est-ce pas ?!... )  |
 | oui mais ce type de spline est plus facile a utiliser kan on fait    |
 |                  bouger les Controls point oki.                      |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/


#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |    Class BSplineAff   |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
BSplinaeAff::BSplinaeAff(U32 nbControlPoints, U32 nbLigneAff) {
    spline = new BSplineU3D(nbControlPoints, 7);

    nbmaxLine = nbLigneAff;
    vec4_set(RVBAStart, 1.0f, 1.0f, 1.0f, 1.0f);
    vec4_set(RVBAEnd, 1.0f, 1.0f, 1.0f, 1.0f);

    //------------- met des valeurs par defaut
    for (U32 a=0; a<nbControlPoints; a++) {
        Ufloat* vec = spline->GetControlPt(a);
        vec3_set(vec, 0.0f, 0.0f, 0.0f);     // position a 0 par defaut
        vec4_set(&vec[3], 1.0f, 1.0f, 1.0f, 1.0f);     // couleur a blanc par defaut
    }

    Spline2d = FALSE;
    Ztest = FALSE;
    Transp = MatTranspAddAlpha;
}
//----------------------------------------------------------------------------------------------------------------------------------------
BSplinaeAff::~BSplinaeAff() {
    delete spline;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void BSplinaeAff::SetKeyPos(U32 num, Ufloat x, Ufloat y, Ufloat z) {
    vec3_set(spline->GetControlPt(num), x, y, z);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void BSplinaeAff::SetKeyPos(U32 num, Ufloat vec[3]) {
    vec3_eg(spline->GetControlPt(num), vec);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void BSplinaeAff::SetKeyColor(U32 num, Ufloat r, Ufloat v, Ufloat b, Ufloat a) {
    Ufloat* key = spline->GetControlPt(num);
    vec4_set(&key[3], r, v, b, a);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void BSplinaeAff::Affiche() {
    Ufloat      destSp[7];
    Ufloat      posSp = 0.0f;
    Ufloat      incSp = 1.f / nbmaxLine;
    Ufloat      RVBA[2][4];
    Ufloat      RVBAstatic[4],RVBAit[4];
    CoordU3D    pt1,pt2;


    vec4_eg(RVBAstatic, RVBAStart);
    vec4_sub(RVBAit, RVBAEnd, RVBAStart);
    vec4_div(RVBAit, RVBAit, nbmaxLine);

    spline->GetVal(destSp, 0.0f);

    vec3_eg(pt2.trans, destSp);
    vec4_eg(RVBA[1], &destSp[3]);
    vec4_mul(RVBA[1], RVBA[1], RVBAstatic);

    U3D3Pipeline->Begin(Transp, FALSE, Ztest);

    for (U32 a=0; a<nbmaxLine; a++) {
        posSp += incSp;

        vec4_add(RVBAstatic, RVBAstatic, RVBAit);

        pt1.frame_process = -1;
        pt2.frame_process = -1;   // permet de recalculer la projection
        spline->GetVal(destSp, posSp);     // prend les valeurs
        vec4_eg(RVBA[0], RVBA[1]);
        vec3_eg(pt1.trans, pt2.trans);
        vec3_eg(pt2.trans, destSp);
        vec4_eg(RVBA[1], &destSp[3]);
        vec4_mul(RVBA[1], RVBA[1], RVBAstatic);

        if (Spline2d)
            U3D3Pipeline->AfficheLigne(pt1.trans, pt2.trans, RVBA);
        else
            U3D3Pipeline->AfficheLigneFromEngine(&pt1, &pt2, RVBA);
    }

    U3D3Pipeline->End(NULL);
}
//----------------------------------------------------------------------------------------------------------------------------------------
