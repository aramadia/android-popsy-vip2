/*-----------------------------------------------------+
 |                       Lignes.cpp                     |
 |                                                      |
 |   Toutes les chtites routines d'affichages de lignes |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------------------------------------
//                                  +---------------------+
//                                  |    Les Fonctions    |
//                                  +---------------------+
//----------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------
static inline bool ClipPlan(Plan* plan, CoordU3D* pt1, CoordU3D* pt2, CoordU3D& pdest1, CoordU3D& pdest2) {
    Ufloat dist1,dist2;
    Ufloat scale;
    Ufloat temp[4];

    dist1 = plan->distance(pt1->trans);
    dist2 = plan->distance(pt2->trans);

    if ((dist1>=0) && (dist2>=0)) {
        vec3_eg(pdest1.trans,  pt1->trans);
        vec4_eg(pdest1.RVBA, pt1->RVBA);

        vec3_eg(pdest2.trans,  pt2->trans);
        vec4_eg(pdest2.RVBA, pt2->RVBA);

        return  TRUE;
    }

    if ((dist1<0) && (dist2<0)) {
        return FALSE;
    }

    if (dist1 >= 0) {
        vec3_eg(pdest1.trans,  pt1->trans);
        vec4_eg(pdest1.RVBA, pt1->RVBA);

        scale =  -dist2 / (dist1 - dist2);

        vec4_sub(temp, pt1->RVBA, pt2->RVBA);
        vec4_mul(temp, temp, scale);
        vec4_add(pdest2.RVBA, temp, pt2->RVBA);

        vec3_sub(temp, pt1->trans, pt2->trans);
        vec3_mul(temp, temp, scale);
        vec3_add(pdest2.trans, temp, pt2->trans);
    } else {
        vec3_eg(pdest2.trans,  pt2->trans);
        vec4_eg(pdest2.RVBA, pt2->RVBA);

        scale =  -dist1 / (dist2 - dist1);

        vec4_sub(temp, pt2->RVBA, pt1->RVBA);
        vec4_mul(temp, temp, scale);
        vec4_add(pdest1.RVBA, temp, pt1->RVBA);

        vec3_sub(temp, pt2->trans, pt1->trans);
        vec3_mul(temp, temp, scale);
        vec3_add(pdest1.trans, temp, pt1->trans);
    }

    return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
static void Interpole(CoordU3D& debut, CoordU3D& fin, Ufloat scale) {
    Ufloat      temp[4];

    vec4_sub(temp, fin.RVBA, debut.RVBA);
    vec4_mul(temp, temp, scale);
    vec4_add(debut.RVBA, temp, debut.RVBA);

    vec4_sub(temp, fin.ecran, debut.ecran);
    vec4_mul(temp, temp, scale);
    vec4_add(debut.ecran, temp, debut.ecran);
}
//----------------------------------------------------------------------------------------------------------------------------------------
static void UpdateYInfo(CoordU3D& pt, Viewport& view) {
    if (pt.ecran[1] < view.GetMinY())
        pt.ClipInfo |= ClipHaut;
    else if (pt.ecran[1] > view.GetMaxY())
        pt.ClipInfo |= ClipBas;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// renvoie une ligne clippée 3D par la camera courante ( renvoie FALSE si ligne non affichable )
//bool GetClippedLineByCamera( CoordU3D &ptdest1, CoordU3D &ptdest2 )
bool GetClippedLineByCamera(CoordU3D& drpt1A, CoordU3D& drpt2A) {
    Camera*     cam = U3D3Monde3D->GetActualCamera();
    CoordU3D    ptdest1,ptdest2;


    //------------------ clip proche si besoin
    if (drpt1A.ClipInfo&ClipProche || drpt2A.ClipInfo&ClipProche) {
        if ((drpt1A.ClipInfo&ClipProche) && (drpt2A.ClipInfo&ClipProche))    // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        vec3_eg(ptdest1.trans, drpt1A.trans);
        vec4_eg(ptdest1.RVBA, drpt1A.RVBA);
        ptdest1.ClipInfo = drpt1A.ClipInfo;

        vec3_eg(ptdest2.trans, drpt2A.trans);
        vec4_eg(ptdest2.RVBA, drpt2A.RVBA);
        ptdest2.ClipInfo = drpt2A.ClipInfo;

        ClipPlan(cam->GetPlanProche(), &ptdest1, &ptdest2, drpt1A, drpt2A);

        cam->Projet(&drpt1A, U3D3Monde3D->GetTick());
        cam->Projet(&drpt2A, U3D3Monde3D->GetTick());
    }



    //----------------- clip chak coin de l'ecran maintenant

    //---------------------- clip sur la gauche
    if (drpt1A.ClipInfo&ClipGauche || drpt2A.ClipInfo&ClipGauche) {
        if (drpt1A.ClipInfo&ClipGauche && drpt2A.ClipInfo&ClipGauche)    // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        vec3_eg(ptdest1.trans, drpt1A.trans);
        vec4_eg(ptdest1.RVBA, drpt1A.RVBA);

        vec3_eg(ptdest2.trans, drpt2A.trans);
        vec4_eg(ptdest2.RVBA, drpt2A.RVBA);

        if (! ClipPlan(cam->GetPlanGauche(), &ptdest1, &ptdest2, drpt1A, drpt2A))
            return FALSE;

        drpt1A.frame_process = (U32)-1;
        drpt2A.frame_process = (U32)-1;

    }

    //---------------------- clip sur la droite
    if (drpt1A.ClipInfo&ClipDroit || drpt2A.ClipInfo&ClipDroit) {
        if (drpt1A.ClipInfo&ClipDroit && drpt2A.ClipInfo&ClipDroit)  // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        vec3_eg(ptdest1.trans, drpt1A.trans);
        vec4_eg(ptdest1.RVBA, drpt1A.RVBA);

        vec3_eg(ptdest2.trans, drpt2A.trans);
        vec4_eg(ptdest2.RVBA, drpt2A.RVBA);

        if (! ClipPlan(cam->GetPlanDroit(), &ptdest1, &ptdest2, drpt1A, drpt2A))
            return FALSE;

        drpt1A.frame_process = (U32)-1;
        drpt2A.frame_process = (U32)-1;
    }

    //---------------------- clip sur le haut
    if (drpt1A.ClipInfo&ClipHaut || drpt2A.ClipInfo&ClipHaut) {
        if (drpt1A.ClipInfo&ClipHaut && drpt2A.ClipInfo&ClipHaut)    // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        vec3_eg(ptdest1.trans, drpt1A.trans);
        vec4_eg(ptdest1.RVBA, drpt1A.RVBA);

        vec3_eg(ptdest2.trans, drpt2A.trans);
        vec4_eg(ptdest2.RVBA, drpt2A.RVBA);

        if (!ClipPlan(cam->GetPlanBas(), &ptdest1, &ptdest2, drpt1A, drpt2A))
            return FALSE;

        drpt1A.frame_process = (U32)-1;
        drpt2A.frame_process = (U32)-1;
    }

    //---------------------- clip sur le bas
    if (drpt1A.ClipInfo&ClipBas || drpt2A.ClipInfo&ClipBas) {
        if (drpt1A.ClipInfo&ClipBas && drpt2A.ClipInfo&ClipBas)  // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        vec3_eg(ptdest1.trans, drpt1A.trans);
        vec4_eg(ptdest1.RVBA, drpt1A.RVBA);

        vec3_eg(ptdest2.trans, drpt2A.trans);
        vec4_eg(ptdest2.RVBA, drpt2A.RVBA);

        if (! ClipPlan(cam->GetPlanHaut(), &ptdest1, &ptdest2, drpt1A, drpt2A))
            return FALSE;

        drpt1A.frame_process = (U32)-1;
        drpt2A.frame_process = (U32)-1;
    }

    cam->Projet(&drpt1A, U3D3Monde3D->GetTick());
    cam->Projet(&drpt2A, U3D3Monde3D->GetTick());

    return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// renvoie une ligne clippée 2D par le viewport ( renvoie FALSE si ligne non affichable )
bool GetClippedLineByViewport(CoordU3D& drpt1A, CoordU3D& drpt2A, Viewport& view) {
    Ufloat  vec[4];
    Ufloat  scale;

    //---------------------- clip sur la gauche
    if (drpt1A.ClipInfo&ClipGauche || drpt2A.ClipInfo&ClipGauche) {
        if (drpt1A.ClipInfo&ClipGauche && drpt2A.ClipInfo&ClipGauche)    // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        if (drpt1A.ecran[0] < view.GetMinX()) {
            scale = (view.GetMinX() - drpt1A.ecran[0]) / (drpt2A.ecran[0] - drpt1A.ecran[0]);

            vec4_sub(vec, drpt2A.RVBA, drpt1A.RVBA);
            vec4_mul(vec, vec, scale);
            vec4_add(drpt1A.RVBA, vec, drpt1A.RVBA);

            vec3_sub(vec, drpt2A.ecran, drpt1A.ecran);
            vec3_mul(vec, vec, scale);
            vec3_add(drpt1A.ecran, vec, drpt1A.ecran);

            UpdateYInfo(drpt1A, view);
        } else {
            scale = (view.GetMinX() - drpt2A.ecran[0]) / (drpt1A.ecran[0] - drpt2A.ecran[0]);

            vec4_sub(vec, drpt1A.RVBA, drpt2A.RVBA);
            vec4_mul(vec, vec, scale);
            vec4_add(drpt1A.RVBA, vec, drpt1A.RVBA);

            vec3_sub(vec, drpt1A.ecran, drpt2A.ecran);
            vec3_mul(vec, vec, scale);
            vec3_add(drpt2A.ecran, vec, drpt2A.ecran);

            UpdateYInfo(drpt2A, view);
        }
    }


    //---------------------- clip sur la droit
    if (drpt1A.ClipInfo&ClipDroit || drpt2A.ClipInfo&ClipDroit) {
        if (drpt1A.ClipInfo&ClipDroit && drpt2A.ClipInfo&ClipDroit)  // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        if (drpt1A.ecran[0] > view.GetMaxX()) {
            scale = (view.GetMaxX() - drpt1A.ecran[0]) / (drpt2A.ecran[0] - drpt1A.ecran[0]);

            vec4_sub(vec, drpt2A.RVBA, drpt1A.RVBA);
            vec4_mul(vec, vec, scale);
            vec4_add(drpt1A.RVBA, vec, drpt1A.RVBA);

            vec3_sub(vec, drpt2A.ecran, drpt1A.ecran);
            vec3_mul(vec, vec, scale);
            vec3_add(drpt1A.ecran, vec, drpt1A.ecran);

            UpdateYInfo(drpt1A, view);
        } else {
            scale = (view.GetMaxX() - drpt2A.ecran[0]) / (drpt1A.ecran[0] - drpt2A.ecran[0]);

            vec4_sub(vec, drpt1A.RVBA, drpt2A.RVBA);
            vec4_mul(vec, vec, scale);
            vec4_add(drpt1A.RVBA, vec, drpt1A.RVBA);

            vec3_sub(vec, drpt1A.ecran, drpt2A.ecran);
            vec3_mul(vec, vec, scale);
            vec3_add(drpt2A.ecran, vec, drpt2A.ecran);

            UpdateYInfo(drpt2A, view);
        }
    }


    //---------------------- clip sur le haut
    if (drpt1A.ClipInfo&ClipHaut || drpt2A.ClipInfo&ClipHaut) {
        if (drpt1A.ClipInfo&ClipHaut && drpt2A.ClipInfo&ClipHaut)    // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        if (drpt1A.ecran[1] < view.GetMinY()) {
            scale = (view.GetMinY() - drpt1A.ecran[1]) / (drpt2A.ecran[1] - drpt1A.ecran[1]);

            vec4_sub(vec, drpt2A.RVBA, drpt1A.RVBA);
            vec4_mul(vec, vec, scale);
            vec4_add(drpt1A.RVBA, vec, drpt1A.RVBA);

            vec3_sub(vec, drpt2A.ecran, drpt1A.ecran);
            vec3_mul(vec, vec, scale);
            vec3_add(drpt1A.ecran, vec, drpt1A.ecran);
        } else {
            scale = (view.GetMinY() - drpt2A.ecran[1]) / (drpt1A.ecran[1] - drpt2A.ecran[1]);

            vec4_sub(vec, drpt1A.RVBA, drpt2A.RVBA);
            vec4_mul(vec, vec, scale);
            vec4_add(drpt1A.RVBA, vec, drpt1A.RVBA);

            vec3_sub(vec, drpt1A.ecran, drpt2A.ecran);
            vec3_mul(vec, vec, scale);
            vec3_add(drpt2A.ecran, vec, drpt2A.ecran);
        }
    }

    //---------------------- clip sur le bas
    if (drpt1A.ClipInfo&ClipBas || drpt2A.ClipInfo&ClipBas) {
        if (drpt1A.ClipInfo&ClipBas && drpt2A.ClipInfo&ClipBas)  // cas ou les 2 pts sont derreier la camera
            return  FALSE;

        if (drpt1A.ecran[1] > view.GetMaxY()) {
            scale = (view.GetMaxY() - drpt1A.ecran[1]) / (drpt2A.ecran[1] - drpt1A.ecran[1]);

            vec4_sub(vec, drpt2A.RVBA, drpt1A.RVBA);
            vec4_mul(vec, vec, scale);
            vec4_add(drpt1A.RVBA, vec, drpt1A.RVBA);

            vec3_sub(vec, drpt2A.ecran, drpt1A.ecran);
            vec3_mul(vec, vec, scale);
            vec3_add(drpt1A.ecran, vec, drpt1A.ecran);
        } else {
            scale = (view.GetMaxY() - drpt2A.ecran[1]) / (drpt1A.ecran[1] - drpt2A.ecran[1]);

            vec4_sub(vec, drpt1A.RVBA, drpt2A.RVBA);
            vec4_mul(vec, vec, scale);
            vec4_add(drpt1A.RVBA, vec, drpt1A.RVBA);

            vec3_sub(vec, drpt1A.ecran, drpt2A.ecran);
            vec3_mul(vec, vec, scale);
            vec3_add(drpt2A.ecran, vec, drpt2A.ecran);
        }
    }

    return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
