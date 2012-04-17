/*-----------------------------------------------------+
 |                      polyprim.h                      |
 |                                                      |
 |   Toutes les primitives d'affichage des polygones    |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _polyprim_U3D3_H
#define _polyprim_U3D3_H


//==================
// Fonctions à appelée car elle font des Preset

void DrawFlatPolyFlatU3D(PolygonU3D* p, Ufloat R, Ufloat V, Ufloat B);
void DrawFlatPolyGouraudU3D(PolygonU3D* p);

void DrawTexturePolyFlatU3D(PolygonU3D* p, Ufloat R, Ufloat V, Ufloat B);
void DrawTexturePolyGouraudU3D(PolygonU3D* p);
void DrawTexturePolyPhongU3D(PolygonU3D* p);

void Draw2TexturePolyU3D(PolygonU3D* p);

#endif
