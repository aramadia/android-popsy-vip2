/*-----------------------------------------------------+
 |              ObjStaticLighting.cpp                   |
 |                                                      |
 |  Une class pour stocker le lighting static d'un      |
 |              objet 3d poil de pied                   |
 |                                                      |
 |  U2^PoPsy TeAm 2000                              |
 +-----------------------------------------------------*/

#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-------------------------------+
//                                  |    Class ObjStaticLighting    |
//                                  +-------------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
ObjStaticLighting::ObjStaticLighting(ObjetU3D* obj) {
    LightU3D*   li;
    Ufloat      ambient[3];

    U3D3Monde3D->GetAmbientColor(ambient);

    GouraudData = new Ufloat[ obj->nbcoords*3 ];

    for (U32 a=0; a<obj->nbcoords; a++)
        vec3_eg(&GouraudData[a*3], ambient);

    //------------------ va calculer le lighting static
    for (U3D3Monde3D->IniLightIterator(); li=U3D3Monde3D->GetLightFromIterator();  U3D3Monde3D->IncLightIterator())
        if (!li->IsAnimated())
            li->CalcGouraudStaticLighting(GouraudData, obj);
}
//----------------------------------------------------------------------------------------------------------
ObjStaticLighting::~ObjStaticLighting() {
    delete [] GouraudData;
}
//----------------------------------------------------------------------------------------------------------
void ObjStaticLighting::GetGouraud(CoordU3D* tab, U32 nb) {
    Ufloat* gd = GouraudData;

    for (U32 a=0; a<nb; a++,tab++,gd+=3)
        vec3_eg(tab->RVBA, gd);
}
//----------------------------------------------------------------------------------------------------------
