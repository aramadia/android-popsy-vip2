/*-----------------------------------------------------+
 |                        U3D3.h                        |
 |                                                      |
 |   C'est le Header de ma plateforme de developpement  |
 |        de U3D3 ( moteur 3d et gestion de moult       |
 |           Truc pour faire de la bonne demo)          |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _U3D3_U3D3_H
#define _U3D3_U3D3_H

typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;
typedef const char* LPCSTR;

#define TRUE true
#define FALSE false
#define WINAPI
#define Sin(x) sin(x)
#define Cos(x) cos(x)
#define Pi() M_PI

#ifdef TOOLU3D
#include "3dsftk.h"     // lib pour lire les .3ds
#endif

#include "OSspec.h"

#include <stdlib.h>
#include <stdio.h>

extern "C" {
    //jpeglib defines a INT32 but it is defined in xmd.h
#define XMD_H

#ifdef __APPLE__
#include <jpeglib.h>
#else	
#include "jpeglib.h"        // depuis Include de la JpegLib
#endif
#undef XMD_H
}
#include "Types.h"
#include "Emess.h"
#include "MathMacro.h"
#include "Vectu3d.h"
#include "MemU3D.h"
#include "mat3x4.h"
#include "M44u3d.h"
#include "quat.h"
#include "TextureU3D.h"
#include "material.h"
#include "Boundings.h"
#include "Plan.h"
#include "Polygon.h"
#include "BTree.h"
#include "bsp.h"
#include "U3Dfile.h"
#include "MotionKeys.h"
#include "Spline.h"
#include "Bspline.h"
#include "Hide.h"
#include "PFustrum.h"
#include "light.h"
#include "camera.h"
#include "CameraConv.h"
#include "UManager.h"
#include "Fpipe.h"
#include "Polyprim.h"
#include "Rasteriz.h"
#include "Bitmap.h"
#include "Video.h"

#include "objet3d.h"
#include "Objet3dWire.h"
#include "ObjStaticLighting.h"
#include "ObjetBones.h"
#include "ObjetConv.h"

#include "LightConv.h"
#include "World3d.h"
#include "World3DConv.h"
#include "TexMana.h"
#include "LmapsManager.h"
#include "Lignes.h"
#include "DataFile.h"
#include "ClockU3D.h"
#include "2Dstuff.h"


void IniEnvironmentU3D3(Ufloat x, Ufloat y);
void KillEnvironmentU3D3();

//TextureManager    *SetActualTextureManager( TextureManager *tex );   // renvoie l'ancien Textremanager
MondeU3D*        SetActualMondeU3D(MondeU3D* monde);     // renvoie l'ancien monde3d
UManager*        SetActualManagerU3D(UManager* manager);     // renvoie l'ancien manager

//==================
// des TRES importantes Variables Globales !

extern UManager*         U3D3Manager;
extern MondeU3D*         U3D3Monde3D;
//extern TextureManager *U3D3TexManager;
extern DataManager*      U3D3DataManager;
extern PolyPipeLine*     U3D3Pipeline;
#ifdef _LMAPS
extern LightmapManager*  U3D3LightMapManager;
#endif

//==================

#endif
