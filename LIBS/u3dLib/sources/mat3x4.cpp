/*-----------------------------------------------------+
 |                      Mat3x4.h                        |
 |                                                      |
 |  Mon implémentation des matrices de transformartions |
 |                  dans le moteur TRidé                |
 |                                                      |
 |  U2^PoPsy TeAm 2000                              |
 +-----------------------------------------------------*/

#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +---------------------+
//                                  |    Class Mat3x4     |
//                                  +---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
Mat3x4::Mat3x4() {
    Identity();
}
//----------------------------------------------------------------------------------------------------------
Mat3x4::Mat3x4(Ufloat m11, Ufloat m12, Ufloat m13,
               Ufloat m21, Ufloat m22, Ufloat m23,
               Ufloat m31, Ufloat m32, Ufloat m33,
               Ufloat xp, Ufloat yp, Ufloat zp) {
    Mat3x4::m11 = m11;
    Mat3x4::m12 = m12;
    Mat3x4::m13 = m13;
    Mat3x4::m21 = m21;
    Mat3x4::m22 = m22;
    Mat3x4::m23 = m23;
    Mat3x4::m31 = m31;
    Mat3x4::m32 = m32;
    Mat3x4::m33 = m33;
    Mat3x4::xp = xp;
    Mat3x4::yp = yp;
    Mat3x4::zp = zp;
}
//----------------------------------------------------------------------------------------------------------
void Mat3x4::Serialize(CArchive& ar) {
    if (ar.IsStoring()) {
        ar << m11;
        ar << m12;
        ar << m13;
        ar << m21;
        ar << m22;
        ar << m23;
        ar << m31;
        ar << m32;
        ar << m33;
        ar << xp;
        ar << yp;
        ar << zp;
    } else {
        ar >> m11;
        ar >> m12;
        ar >> m13;
        ar >> m21;
        ar >> m22;
        ar >> m23;
        ar >> m31;
        ar >> m32;
        ar >> m33;
        ar >> xp;
        ar >> yp;
        ar >> zp;
    }
}
//----------------------------------------------------------------------------------------------------------
void Mat3x4::Set(Ufloat m11, Ufloat m12, Ufloat m13,
                 Ufloat m21, Ufloat m22, Ufloat m23,
                 Ufloat m31, Ufloat m32, Ufloat m33,
                 Ufloat xp, Ufloat yp, Ufloat zp) {
    Mat3x4::m11 = m11;
    Mat3x4::m12 = m12;
    Mat3x4::m13 = m13;
    Mat3x4::m21 = m21;
    Mat3x4::m22 = m22;
    Mat3x4::m23 = m23;
    Mat3x4::m31 = m31;
    Mat3x4::m32 = m32;
    Mat3x4::m33 = m33;
    Mat3x4::xp = xp;
    Mat3x4::yp = yp;
    Mat3x4::zp = zp;
}
//----------------------------------------------------------------------------------------------------------
void Mat3x4::Set(U32 ligne, Ufloat m1, Ufloat m2, Ufloat m3) {
    switch (ligne) {
    case 0:
        m11 = m1;
        m12 = m2;
        m13 = m3;
        break;
    case 1:
        m21 = m1;
        m22 = m2;
        m23 = m3;
        break;
    case 2:
        m31 = m1;
        m32 = m2;
        m33 = m3;
        break;
    }
}
//----------------------------------------------------------------------------------------------------------
//void Mat3x4::Set( U32 ligne, const Ufloat vec[3] )
//{
//  switch( ligne )
//  {
//      case 0:
//          m11 = vec[0];
//          m12 = vec[1];
//          m13 = vec[2];
//          break;
//      case 1:
//          m21 = vec[0];
//          m22 = vec[1];
//          m23 = vec[2];
//          break;
//      case 2:
//          m31 = vec[0];
//          m32 = vec[1];
//          m33 = vec[2];
//          break;
//  }
//}
//----------------------------------------------------------------------------------------------------------
void Mat3x4::Identity() {
    m11 = 1.0f;
    m12 = 0.0f;
    m13 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
    m23 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;
    xp  = 0.0f;
    yp  = 0.0f;
    zp  = 0.0f;
}
//----------------------------------------------------------------------------------------------------------
void Mat3x4::Transpose() {
    Ufloat transp;

    transp = m12;
    m12 = m21;
    m21 = transp;
    transp = m13;
    m13 = m31;
    m31 = transp;
    transp = m23;
    m23 = m32;
    m32 = transp;
}
//----------------------------------------------------------------------------------------------------------
Mat3x4 Mat3x4::GetInverse() {
    Mat3x4 C(
        (m22*m33 - m23*m32), -(m12*m33 - m13*m32), (m12*m23 - m13*m22),
        -(m21*m33 - m23*m31), (m11*m33 - m13*m31), -(m11*m23 - m13*m21),
        (m21*m32 - m22*m31), -(m11*m32 - m12*m31), (m11*m22 - m12*m21));

    Ufloat s = (Ufloat)1./(m11*C.m11 + m12*C.m21 + m13*C.m31);

    C *= s;

    C.xp = -(xp*C.m11 + yp*C.m12 + zp*C.m13);
    C.yp = -(xp*C.m21 + yp*C.m22 + zp*C.m23);
    C.zp = -(xp*C.m31 + yp*C.m32 + zp*C.m33);

    return C;
}
//----------------------------------------------------------------------------------------------------------
float Mat3x4::Determinant() {
    return m11 * (m22*m33 - m23*m32) -m12 * (m21*m33 - m23*m31) +m13 * (m21*m32 - m22*m31);
}
//----------------------------------------------------------------------------------------------------------
Mat3x4& Mat3x4::operator*=(const Mat3x4& m) {
    Mat3x4  tmp;

    tmp.m11 = m.m11*m11 + m.m12*m21 + m.m13*m31;
    tmp.m12 = m.m11*m12 + m.m12*m22 + m.m13*m32;
    tmp.m13 = m.m11*m13 + m.m12*m23 + m.m13*m33;
    tmp.m21 = m.m21*m11 + m.m22*m21 + m.m23*m31;
    tmp.m22 = m.m21*m12 + m.m22*m22 + m.m23*m32;
    tmp.m23 = m.m21*m13 + m.m22*m23 + m.m23*m33;
    tmp.m31 = m.m31*m11 + m.m32*m21 + m.m33*m31;
    tmp.m32 = m.m31*m12 + m.m32*m22 + m.m33*m32;
    tmp.m33 = m.m31*m13 + m.m32*m23 + m.m33*m33;

    tmp.xp = m.m11*xp + m.m12*yp + m.m13*zp + m.xp;
    tmp.yp = m.m21*xp + m.m22*yp + m.m23*zp + m.yp;
    tmp.zp = m.m31*xp + m.m32*yp + m.m33*zp + m.zp;

    *this = tmp;

    return *this;
}
//----------------------------------------------------------------------------------------------------------
Mat3x4& Mat3x4::operator*=(float s) {
    m11 *= s;
    m12 *= s;
    m13 *= s;
    m21 *= s;
    m22 *= s;
    m23 *= s;
    m31 *= s;
    m32 *= s;
    m33 *= s;

    return *this;
}
//----------------------------------------------------------------------------------------------------------
void Mat3x4::GetTransfo(Ufloat vdest[3], const Ufloat vsrce[3]) {
    vdest[0] = GetXTransfo(vsrce);
    vdest[1] = GetYTransfo(vsrce);
    vdest[2] = GetZTransfo(vsrce);
}
//----------------------------------------------------------------------------------------------------------
// Transfo pour un vecteur
void Mat3x4::GetTransfoR(Ufloat vdest[3], const Ufloat vsrce[3]) {
    vdest[0] = GetXTransfoR(vsrce);
    vdest[1] = GetYTransfoR(vsrce);
    vdest[2] = GetZTransfoR(vsrce);
}
//----------------------------------------------------------------------------------------------------------
void Mat3x4::GetTranspose(Ufloat vdest[3], const Ufloat vsrce[3]) {
    Ufloat ftmp[3];
    Mat3x4 Mtmp;

    vec3_eg(ftmp, vsrce);
    ftmp[0] -=  xp;
    ftmp[1] -=  yp;
    ftmp[2] -=  zp;

    Mtmp = *this;
    Mtmp.Transpose();

    vdest[0] = Mtmp.GetXTransfoR(ftmp);
    vdest[1] = Mtmp.GetYTransfoR(ftmp);
    vdest[2] = Mtmp.GetZTransfoR(ftmp);
}
//----------------------------------------------------------------------------------------------------------
// donne le transposé ( pour un vecteur )
void Mat3x4::GetTransposeR(Ufloat vdest[3], const Ufloat vsrce[3]) {
    Mat3x4 Mtmp;

    Mtmp = *this;
    Mtmp.Transpose();

    vdest[0] = Mtmp.GetXTransfoR(vsrce);
    vdest[1] = Mtmp.GetYTransfoR(vsrce);
    vdest[2] = Mtmp.GetZTransfoR(vsrce);
}
//----------------------------------------------------------------------------------------------------------
