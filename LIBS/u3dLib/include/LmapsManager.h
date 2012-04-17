/*-----------------------------------------------------+
 |                     LmapsManager.h                   |
 |                                                      |
 |   Bon ca va g�rer sec les lightmaps ici alors faites |
 |                     attention !!!                    |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifdef _LMAPS
#ifndef _LmapsManager_U3D3_H
#define _LmapsManager_U3D3_H





//======================
// d�finition du manager de lightmap
class LightmapManager {

    Lightmap* liste;
    U32 nbElem;

    void Add(Lightmap* l);

public:

    LightmapManager();
    ~LightmapManager();

    // renvoie une nouvelle lightmap pour le poly donn�
    Lightmap* GetNew(PolygonU3D& p);

    // va calculer le lighting sur la lightmap du poly !
    void CalculLightMap(PolygonU3D& p);

};

#endif
#endif
