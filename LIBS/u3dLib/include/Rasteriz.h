/*-----------------------------------------------------+
 |                       Rasteriz.h                     |
 |                                                      |
 |  c'est ici que l'on va prendre chaque poly analyser  |
 |  leur propriété de material et l'afficher a l'ecran  |
 |              en fonction de cela!                    |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _Rasteriz_U3D3_H
#define _Rasteriz_U3D3_H


//=============================
// une petites structure de depannage ki va bien

typedef struct {

    Ufloat RVBA[4];
    Ufloat UV[2];
    Ufloat UV1[2];
    Ufloat XYZW[4];

} Raster2DData;


void Rasteriz(PolygonU3D* p);       // rasteriz le domaine BSP
void RasterizNoTexture(PolygonU3D* p);       // draw 1 poly sans texture
void RasterizTexture(PolygonU3D* p);       // draw 1 poly avec texture
void RasterizMultiTexture(PolygonU3D* p);       // draw 1 poly avec  2 textures
void Rasteriz3DLines(PolygonU3D* p);       // draw les lignes d'un poly

#endif
