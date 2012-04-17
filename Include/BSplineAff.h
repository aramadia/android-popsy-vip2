/*---------------------------------------------------------------------+
 |                          BSplineAff.h                                |
 |                                                                      |
 |  va afficher une gnolie spline avec des lignes toute antialiasées    |
 |      et accélérées ( ca ressemble a HSplineAFF n'est-ce pas ?!... )  |
 | oui mais ce type de spline est plus facile a utiliser kan on fait    |
 |                  bouger les Controls point oki.                      |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#ifndef _BSPLINEAFF_VIP2_H
#define _BSPLINEAFF_VIP2_H

//////////////////////////////////////////
class BSplinaeAff {

//-------------------- Datas
private:

    BSplineU3D* spline;

    U32     nbmaxLine;   // nombre maxi de ligne
    Ufloat  RVBAStart[4];   // couleur de départ de la courbe
    Ufloat  RVBAEnd[4];   // couleur d'arrivée

    bool Spline2d;   // FALSE par defaut
    bool Ztest;   // FALSE par defaut
    U32  Transp;   // MatTranspAddAlpha par defaut

//-------------------- fonctions
public:

    BSplinaeAff(U32 nbControlPoints, U32 nbLigneAff=10);
    ~BSplinaeAff();

    void SetKeyPos(U32 num, Ufloat x, Ufloat y, Ufloat z=0.0f);
    void SetKeyPos(U32 num, Ufloat vec[3]);
    void SetKeyColor(U32 num, Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f);

    void SetZtest(bool b)                                              {
        Ztest=b;
    }
    void SetTransp(U32 tr)                                         {
        Transp=tr;
    }
    void SetSpline2d(bool b)                                           {
        Spline2d=b;
    }
    void SetStartColor(Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f)    {
        vec4_set(RVBAStart,r,v,b,a);
    }
    void SetEndColor(Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f)      {
        vec4_set(RVBAEnd,r,v,b,a);
    }
    void SetStartAlpha(Ufloat a)                                       {
        RVBAStart[3]=a;
    }
    void SetEndAlpha(Ufloat a)                                     {
        RVBAEnd[3]=a;
    }

    void GetVal(Ufloat* destSp, Ufloat pos) {
        spline->GetVal(destSp,pos);
    }

    void Affiche();
};
//////////////////////////////////////////

#endif

