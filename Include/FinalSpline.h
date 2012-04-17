/*-------------------------------------------------------------------------+
 |                          FinalSpline.h                                   |
 |                                                                          |
 |      je vasi tracer des Bspline les long des lettres de VIP2 na !        |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _FINALSPLINE_VIP2_H
#define _FINALSPLINE_VIP2_H

/////////////////////////////////////////
class FinalOneSpline {

//--------------- Datas
private:

    ObjetU3D* obj;

    U32 IndexDepart;
    U32 IncrementIndex;

    BSplinaeAff*    spline;

    Ufloat TimeReste;
    Ufloat ActuaTime;

public:

    FinalOneSpline* suiv;

//--------------- Fonctions
private:

    void AssignKey();

public:

    FinalOneSpline(ObjetU3D* obj);
    ~FinalOneSpline();

    void SeAlpha(Ufloat a);

    void UpdateFrame(Ufloat laptime);
};
/////////////////////////////////////////





/////////////////////////////////////////
class FinalSpline {

//--------------- Datas
private:

    FinalOneSpline* AllSpline;

    Ufloat TAppear;
    Ufloat actuatime;

//--------------- Fonctions
public:

    FinalSpline(MondeU3D* monde);
    ~FinalSpline();

    void Appear(Ufloat time);

    void UpdateFrame(Ufloat laptime);
};
/////////////////////////////////////////

#endif
