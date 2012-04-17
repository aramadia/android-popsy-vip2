/*---------------------------------------------------------+
 |                    vip2Headers.h                         |
 |                                                          |
 |     tout les headers nécessaires a la demo ( po po po )  |
 |                                                          |
 |  U2^PoPsy TeAm 2000                                  |
 +--------------------------------------------------------*/

#ifndef _VIP2HEADERS_H_
#define _VIP2HEADERS_H_

#define FRAMEPERSECOND  30. // nombre de frame jouées par secondes kan on play une scene U3D

#define DEFAULT_SCREEN_X    640     // coord X par defaut de l'ecran
#define DEFAULT_SCREEN_Y    480     // coord Y par defaut de l'ecran

#include "U3D3.h"

#include "mp3.h"
#include "linux_oss.h"
#include "Utils.h"
#include "Scheduler.h"
#include "Synchronisater.h"
#include "Image.h"
#include "ImageBlur.h"
#include "HSplineAff.h"
#include "BSplineAff.h"
#include "ObjDeformation.h"
#include "XGrezilles.h"
#include "BPeloche.h"

#include "NebuPart.h"
#include "NebuTexte.h"
#include "Nebuleuse.h"

#define NEURO_TIME_BEGIN 30.790f
#include "NeuroLights.h"
#include "NeuroLines.h"
#include "NeuroBack.h"
#include "NeuroTexte.h"
#include "Neurone.h"
#define NEURO_TIME_END 53.590f

#define PLANS_TIME_BEGIN 52.66f
#include "PlansTextes.h"
#include "PlansCache.h"
#include "PlansAnim.h"
#include "PlansClock.h"
#include "PlansTransp.h"
#include "Plans.h"
#define PLANS_TIME_END 72.76f

#define OEIL_TIME_BEGIN 67.95f
#include "OeilLignes.h"
#include "OeilDefor.h"
#include "OeilTextes.h"
#include "Oeil.h"
#define OEIL_TIME_END 86.85f


#define CILS_TIME_BEGIN 85.34f
#include "CilsBoule.h"
#include "CilsCarotus.h"
#include "CilsBack.h"
#include "CilsTextes.h"
#include "Cils.h"
#define CILS_TIME_END 96.16f


#define GHOST_TIME_BEGIN 93.31f
#include "GhostEtincelle.h"
#include "GhostEclaires.h"
#include "GhostVPort.h"
#include "Ghost2D.h"
#include "Ghost.h"
#define GHOST_TIME_END 111.54f

#include "EtoileScale.h"
#include "EtoileGreets.h"
#include "Etoile.h"


#define COUPLE_TIME_BEGIN 111.74f
#include "CouplePetals.h"
#include "CouplePetals3D.h"
#include "CoupleTextes.h"
#include "CoupleRPluie.h"
#include "CouplePluieL.h"
#include "CouplePluieP.h"
#include "Couple.h"
#define COUPLE_TIME_END 138.98f

#define MULTI_TIME_BEGIN 138.98f
#include "Multi.h"
#define MULTI_TIME_END 164.836f

#include "ColonneAnim.h"
#include "Colonne.h"

#include "LettresBack.h"
#include "Lettres.h"

#define SMILE_TIME_BEGIN 164.836f
#include "Smile.h"
#define SMILE_TIME_END 169.836f

#define FINAL_TIME_BEGIN 164.836f
#include "FinalSpline.h"
#include "FinalTexte.h"
#include "Final.h"
#define FINAL_TIME_END 197.836f


extern Udouble          LaunchTime;   // le temps ou la demo a démarré
extern USynchronisater* AllSynchro;   // toute les synchros de la demo
extern OSSAudioDriver* mp3;

//-------- Interface
#include <math.h>
#include "ElemAff.h"
#include "Render.h"

#endif
