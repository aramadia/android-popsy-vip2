/*-----------------------------------------------------------------+
 |                      CouplePetals3D.h                            |
 |                                                                  |
 |  je vasi essayer de faire des petals de fleurs sur la 2eme scene |
 |              du couple, avec un chti doigt de physik             |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _COUPLEPETALS3D_VIP2_H_
#define _COUPLEPETALS3D_VIP2_H_

//////////////////////////////////////////////
class CoupleOnePetals3D:public CoupleOnePetals {

//------------------ Datas
private:

    PolygonU3D* p1,*p2;
    CoordU3D*   coord[6];

    Quat rot,incrot;   // parametre de rotation du petal

//------------------ Fonctions
public:

    CoupleOnePetals3D();

    void Set(PolygonU3D* poly, VertexU3D* vertex, MaterialU3D* mat);     // les 2 poly et les 6 vertx doivent etre a la suite
    void Ini(Ufloat tapp, Ufloat tres, Ufloat tdisa, Ufloat pos[3], Ufloat vitesse[3]);

    Quat& GetQuat() {
        return rot;
    }

    void IncAnim(Ufloat laptime);

};
//////////////////////////////////////////////

//////////////////////////////////////////////
class CoupleShadowP3D {

//------------------ Datas
private:

    PolygonU3D*         p1;
    CoupleOnePetals3D*  petal;

public:

    CoupleShadowP3D* suiv;

//------------------ Fonctions
public:

    CoupleShadowP3D();

    void Set(PolygonU3D* poly, VertexU3D* vertex, MaterialU3D* mat, CoupleOnePetals3D* petal);     // les 2 poly et les 6 vertx doivent etre a la suite

    void IncAnim(Ufloat laptime);
};
//////////////////////////////////////////////

#endif

