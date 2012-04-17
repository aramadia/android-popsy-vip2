/*-----------------------------------------------------+
 |                       Plan.h                         |
 |                                                      |
 |     c'est fou comme les plans sont utils en 3D       |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _PLAN_U3D3_H
#define _PLAN_U3D3_H


///////////////////////////////////////////////////////////
// definition d'un plan
typedef enum {DEVANT,DERRIERE,ENTRE_DEUX} BoundPlace;

struct Plan {

    Ufloat normal[3];   // normale
    Ufloat dist;        // distance sur la normale

public :

    Plan()                                  {
        vec3_set(normal, 0.f, 0.f, 0.f);
        dist=0.f;
    }
    Plan(Ufloat vec[3], Ufloat pos[3]);

    Ufloat  distance(const Ufloat v[3])                                             {
        return vec3_dot(v, normal) - dist;
    }
    void    Calc(const Ufloat pt0[3], const Ufloat pt1[3], const Ufloat pt2[3]);     // recalcul le plan à partie des 3 points
    void    CalcRT(const Ufloat pt0[3], const Ufloat pt1[3], const Ufloat pt2[3]);
    void    GetTransMatrix(Mat3x4& m);   // renvoie la matrice de transformation associée au plan

    Plan& operator*=(Mat3x4& m);   // applique matrice au plan
    /*  //renvoie position d'une Bounding box par rapport au plan
        BoundPlace GetPos( BoundingBox &b );*/
};
///////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------------------
//                                  +------------------------+
//                                  |  Les Fonctions Inline  |
//                                  +------------------------+
//----------------------------------------------------------------------------------------------------------
void inline Plan::CalcRT(const Ufloat pt0[3], const Ufloat pt1[3], const Ufloat pt2[3]) {
    Ufloat v1[3],v2[3];

    vec3_sub(v1, pt1, pt0);
    vec3_sub(v2, pt2, pt0);
    vec3_xprd(normal, v1, v2);
    vec3_FastNormalize(normal);
    dist = vec3_dot(pt0, normal);
}
//----------------------------------------------------------------------------------------------------------

#endif
