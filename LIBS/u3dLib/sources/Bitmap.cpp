/*-----------------------------------------------------+
 |                      Bitmap.cpp                      |
 |                                                      |
 |  des classes pour gérer efficacement et facilement   |
 |          l'affichage d'image 2D avec OpenGL          |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +---------------------+
//                                  |    Class Bitmap     |
//                                  +---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
BitmapU3D::BitmapU3D(MaterialU3D* m, U32 f) {
    flag = f;

    mat = m;

    for (U32 a=0; a<4; a++)
        vec4_set(RVBA[a], 1.0f, 1.0f, 1.0f, 1.0f);

    vec3_set(XYZ[0], 0.0f, 0.0f, ProcheZClipping+0.1f);
    vec3_set(XYZ[1], 1.0f, 0.0f, ProcheZClipping+0.1f);
    vec3_set(XYZ[2], 1.0f, 1.0f, ProcheZClipping+0.1f);
    vec3_set(XYZ[3], 0.0f, 1.0f, ProcheZClipping+0.1f);

    vec4_set(UV[0], 0.0f, 0.0f, 0.0f, 0.0f);
    vec4_set(UV[1], 1.0f, 0.0f, 1.0f, 0.0f);
    vec4_set(UV[2], 1.0f, 1.0f, 1.0f, 1.0f);
    vec4_set(UV[3], 0.0f, 1.0f, 0.0f, 1.0f);
}
//----------------------------------------------------------------------------------------------------------
BitmapU3D& BitmapU3D::operator=(BitmapU3D& bmp) {
    flag = bmp.flag;

    mat = bmp.mat;

    for (U32 a=0; a<4; a++) {
        vec4_eg(RVBA[a], bmp.RVBA[a]);
        vec4_eg(XYZ[a], bmp.XYZ[a]);
        vec4_eg(UV[a], bmp.UV[a]);
    }

    return *this;
}
//----------------------------------------------------------------------------------------------------------
// met une couleur pour toute l'image
void BitmapU3D::SetColor(Ufloat R, Ufloat V, Ufloat B, Ufloat A) {
    for (U32 a=0; a<4; a++)
        vec4_set(RVBA[a], R, V, B, A);
}
//----------------------------------------------------------------------------------------------------------
// met une couleur pour toute l'image
void BitmapU3D::SetColor(Ufloat col[4]) {
    for (U32 a=0; a<4; a++)
        vec4_eg(RVBA[a], col);
}
//----------------------------------------------------------------------------------------------------------
// met une couleur pour le sommet n
void BitmapU3D::SetColor(U32 sommet, Ufloat col[4]) {
    vec4_eg(RVBA[sommet], col);
}
//----------------------------------------------------------------------------------------------------------
// met une couleur pour le sommet n
void BitmapU3D::SetColor(U32 sommet, Ufloat R, Ufloat V, Ufloat B, Ufloat A) {
    vec4_set(RVBA[sommet], R, V, B, A);
}
//----------------------------------------------------------------------------------------------------------
// met une coord de mapping pour le sommet n
void BitmapU3D::SetMapCoord(U32 sommet, Ufloat u, Ufloat v, U32 stage) {
    stage<<=1;

    UV[sommet][ 0 + stage ] = u;
    UV[sommet][ 1 + stage ] = v;
}
//----------------------------------------------------------------------------------------------------------
void BitmapU3D::SetCoord(U32 sommet, Ufloat X, Ufloat Y, Ufloat Z) {
    vec3_set(XYZ[sommet], X, Y, Z);
}
//----------------------------------------------------------------------------------------------------------
void BitmapU3D::SetCoord(U32 sommet, Ufloat xyz[3]) {
    vec3_eg(XYZ[sommet], xyz);
}
//----------------------------------------------------------------------------------------------------------
