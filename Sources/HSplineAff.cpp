/*---------------------------------------------------------------------+
 |                          HSplineAff.cpp                              |
 |                                                                      |
 |  va afficher une gnolie spline avec des lignes toute antialiasées    |
 |                      et accélérées                                   |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |    Class HSplineAff   |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
HSplinaeAff::HSplinaeAff(bool spline2d) {
    tempKeys = NULL;
    actuaKey = NULL;
    nbKey = 0;   // nombre de clé

    spline = NULL;

    nbmaxLine = 0;
    Tpos = 0.0f;
    Tmax = 0.0f;
    vec4_set(RVBAStart, 1.0f, 1.0f, 1.0f, 1.0f);
    vec4_set(RVBAEnd, 1.0f, 1.0f, 1.0f, 1.0f);

    Spline2d = spline2d;
    Ztest = FALSE;
    Transp = MatTranspAddAlpha;
}
//----------------------------------------------------------------------------------------------------------------------------------------
HSplinaeAff::~HSplinaeAff() {
    KillTempKeys();

    if (spline) delete spline;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void HSplinaeAff::KillTempKeys() {
    HSplinaeAff_stock* tmp;

    while (tempKeys) {
        tmp = tempKeys->suiv;
        delete tempKeys;
        tempKeys = tmp;
    }

    nbKey = 0;   // nombre de clé
}
//----------------------------------------------------------------------------------------------------------------------------------------
void HSplinaeAff::AddKey(Ufloat time) {
    HSplinaeAff_stock* tmp,*preced=NULL;


    actuaKey = new HSplinaeAff_stock;
    nbKey++;

    actuaKey->time = time;
    if (time >= Tmax) Tmax = time;    // update Tmax


    //-------------- va rajouter la clé dans l'ordre correct de temps
    tmp = tempKeys;
    while (tmp) {
        if (tmp->time>= actuaKey->time)
            break;
        preced = tmp;
        tmp = tmp->suiv;
    }

    if (preced) {
        preced->suiv = actuaKey;
        actuaKey->suiv = tmp;
    } else {
        actuaKey->suiv = tempKeys;
        tempKeys = actuaKey;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void HSplinaeAff::SetKeyPos(Ufloat x, Ufloat y, Ufloat z) {
    actuaKey->x = x;
    actuaKey->y = y;
    actuaKey->z = z;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void HSplinaeAff::SetKeyColor(Ufloat r, Ufloat v, Ufloat b, Ufloat a) {
    actuaKey->r = r;
    actuaKey->v = v;
    actuaKey->b = b;
    actuaKey->a = a;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// pré-calcul de la spline ( en donnat un nombre maxi de ligne pour afficher la spline )
void HSplinaeAff::Confirm(U32 nbMaxline) {
    Ufloat*             keys;
    KeyHeaderU3D*       headers;
    HSplinaeAff_stock*  tmp = tempKeys;

    if (spline) delete spline;

    vec4_set(RVBAStart, 1.0f, 1.0f, 1.0f, 1.0f);
    vec4_set(RVBAEnd, 1.0f, 1.0f, 1.0f, 1.0f);

    nbmaxLine = nbMaxline;
    Tpos = 0.0f;

    keys = new Ufloat[7*nbKey];

    headers = new KeyHeaderU3D[nbKey];

    //----------- va remplir les tableaux temporaires
    U32 dep=0;
    U32 dep1=0;
    while (tmp) {
        keys[dep] = tmp->x;
        dep++;
        keys[dep] = tmp->y;
        dep++;
        keys[dep] = tmp->z;
        dep++;
        keys[dep] = tmp->r;
        dep++;
        keys[dep] = tmp->v;
        dep++;
        keys[dep] = tmp->b;
        dep++;
        keys[dep] = tmp->a;
        dep++;

        headers[dep1].Ini();
        headers[dep1].time = tmp->time;
        dep1++;

        tmp = tmp->suiv;
    }

    spline = new SplineU3D(nbKey, 7, keys, headers);

    delete headers;
    delete keys;
    KillTempKeys();   // on n'en a plus besoin
}
//----------------------------------------------------------------------------------------------------------------------------------------
void HSplinaeAff::GetLastPos(Ufloat dest[3]) {
    Ufloat  destSp[7];

    spline->GetVal(destSp);
    vec3_eg(dest, destSp);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void HSplinaeAff::Affiche(Ufloat laptime) {
    Ufloat      SPpos;
    Ufloat      ITpos;
    Ufloat      destSp[7];
    Ufloat      RVBA[2][4];
    Ufloat      RVBAstatic[4],RVBAit[4];
    U32         nbligne;
    CoordU3D    pt1,pt2;

    //------- calcul position dans la spline
    Tpos += laptime;
    SPpos = Tpos / Tmax;
    if (SPpos > 1.0f) {
        SPpos = 1.0f;
        nbligne = nbmaxLine;
        ITpos = Tmax / nbligne;
    } else {
//      nbligne = SPpos*nbmaxLine;
        nbligne = nbmaxLine;
        if (nbligne < 1)    nbligne = 1;
        ITpos = Tpos / nbligne;
    }


    vec4_eg(RVBAstatic, RVBAStart);
    vec4_sub(RVBAit, RVBAEnd, RVBAStart);
    vec4_div(RVBAit, RVBAit, nbligne);

    spline->Ini();   // démarre du debut ( logik non ? )
    spline->GetVal(destSp);

    vec3_eg(pt2.trans, destSp);
    vec4_eg(RVBA[1], &destSp[3]);
    vec4_mul(RVBA[1], RVBA[1], RVBAstatic);

    U3D3Pipeline->Begin(Transp, FALSE, Ztest);

    for (U32 a=0; a<nbligne; a++) {
        vec4_add(RVBAstatic, RVBAstatic, RVBAit);

        pt1.frame_process = -1;
        pt2.frame_process = -1;   // permet de recalculer la projection
        spline->IncPos(ITpos);     // incrémente position dans la spline
        spline->GetVal(destSp);     // prend les valeurs
        vec4_eg(RVBA[0], RVBA[1]);
        vec3_eg(pt1.trans, pt2.trans);
        vec3_eg(pt2.trans, destSp);
        vec4_eg(RVBA[1], &destSp[3]);
        vec4_mul(RVBA[1], RVBA[1], RVBAstatic);

        U3D3Pipeline->AfficheLigneFromEngine(&pt1, &pt2, RVBA);
    }

    U3D3Pipeline->End(NULL);
}
//----------------------------------------------------------------------------------------------------------------------------------------
