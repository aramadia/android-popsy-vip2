/*-----------------------------------------------------+
 |                       Video.cpp                      |
 |                                                      |
 | on s'occupe ici de la gestion des modes videos       |
 | c'est a dire du passage dans tel ou tel mode etc..  |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/


#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------
//                              +------------------------------+
//                              |    Les Variables globales    |
//                              +------------------------------+
//----------------------------------------------------------------------------------------------------------


//static RECT *ActualRec;       // Rectangle de la fenetre

static bool fenetre;        // mode fenetré ?
static bool Actif;          // est-ce que le mode video est actif
static bool AfficheOK;      // est-ce qu'on peut Afficher qq chose ?
U32  Xres,Yres;             // résolution X et Y demandée
U32  ABpp;                  // nombre de bits par pixels
//HDC HdcMain;


//----------------------------------------------------------------------------------------------------------
//                                  +---------------------+
//                                  |    Les Fonctions    |
//                                  +---------------------+
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
// mode fenétré
void SetVideoW(U32 X, U32 Y) {
    AfficheOK = FALSE;
    fenetre = TRUE;
    Xres = X;
    Yres = Y;

    //EnableOpenGL();

    Actif = TRUE;
}
//----------------------------------------------------------------------------------------------------------
// pour le mode fentre seulement!
#if 0
void SetWindowDC(HDC Hdc) {
    HdcMain = Hdc;
}
#endif
//----------------------------------------------------------------------------------------------------------
// Quit le mode vidéo désiré
void KillVideo() {
//  DisableOpenGL();
//  ChangeDisplaySettings ( NULL, 0 );
}
//----------------------------------------------------------------------------------------------------------
void AfficheFrameU3D() {
//    MSG         msg;

    // gere les messages et attend eventuéllement que l'image
    // précedente ait finit de s'afficher
    /*  do
        {*/
#if 0
    if (PeekMessage(&msg , NULL , 0 , 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
#endif
//  }while( AfficheOK );

    FlipOpenGL();

    AfficheOK = TRUE;
}
//----------------------------------------------------------------------------------------------------------
// essaye de passer en resolution x:y avec le + de couleur possible
bool ChangeScreenResolution(U32 X, U32 Y, U32 bitperpixel) {
#if 0
    bool        ret = TRUE;
    DEVMODE     mode;
    U32         result;       //Result of ChangeDisplaySettings
    U32         num_mode;

    memset(&mode, 0, sizeof(PDEVMODE));
    num_mode = 0;


    //------------------------- Essaye de passer en 32 bit
    if ((bitperpixel==0) || (bitperpixel==32)) {
        while (ret == TRUE) {
            ret = EnumDisplaySettings(NULL,              // use default device
                                      num_mode,       // DEVMODEs start at 0
                                      &mode);       // if successful the system fills in the DEVMODE structure
            num_mode++;

            if (mode.dmBitsPerPel == 32 &&
                    mode.dmPelsWidth  == X &&
                    mode.dmPelsHeight == Y) {
                result = ChangeDisplaySettings(&mode, CDS_FULLSCREEN);     // YEAHHHHHHHHHH!!!!!!!!!!!!!!!!!!!
                if (result != DISP_CHANGE_SUCCESSFUL) {
                    ErrorMess("It's Impossible To change de resolution du con.\n");
                    return FALSE;
                }
                return TRUE;
            }
        }
    }


    //------------------------- sinon essaye de passer en 24 bit
    ret = TRUE;
    num_mode = 0;
    if ((bitperpixel==0) || (bitperpixel==24)) {
        while (ret == TRUE) {
            ret = EnumDisplaySettings(NULL,              // use default device
                                      num_mode,       // DEVMODEs start at 0
                                      &mode);       // if successful the system fills in the DEVMODE structure
            num_mode++;

            if (mode.dmBitsPerPel == 24 &&
                    mode.dmPelsWidth  == X &&
                    mode.dmPelsHeight == Y) {
                result = ChangeDisplaySettings(&mode, CDS_FULLSCREEN);     // YEAHHHHHHHHHH!!!!!!!!!!!!!!!!!!!
                if (result != DISP_CHANGE_SUCCESSFUL) {
                    ErrorMess("It's Impossible To change de resolution du con.\n");
                    return FALSE;
                }
                return TRUE;
            }
        }
    }


    //------------------------- Essaye de passer en 16 bit
    ret = TRUE;
    num_mode = 0;
    if ((bitperpixel==0) || (bitperpixel==16)) {
        while (ret == TRUE) {
            ret = EnumDisplaySettings(NULL,              // use default device
                                      num_mode,       // DEVMODEs start at 0
                                      &mode);       // if successful the system fills in the DEVMODE structure
            num_mode++;

            if (mode.dmBitsPerPel == 16 &&
                    mode.dmPelsWidth  == X &&
                    mode.dmPelsHeight == Y) {
                result = ChangeDisplaySettings(&mode, CDS_FULLSCREEN);     // YEAHHHHHHHHHH!!!!!!!!!!!!!!!!!!!
                if (result != DISP_CHANGE_SUCCESSFUL) {
                    ErrorMess("It's Impossible To change de resolution du con.\n");
                    return FALSE;
                }
                return TRUE;
            }
        }
    }


    ErrorMess("Haaaaaaaaaa True Color Mode not Found\n");

    return FALSE;
#endif
}
//----------------------------------------------------------------------------------------------------------
// remet la resolution comme avant
void RestoreScreenResolution() {
//  ChangeDisplaySettings ( NULL, 0 );
}
//----------------------------------------------------------------------------------------------------------
