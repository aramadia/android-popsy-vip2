#include <stdafx.h>
#include <glwindow.h>

#define MP3_PATH    "Datas/zic/vip2.mp3"
// #define NOMP3 1  // remove the sound

//----------------------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// Variables globales
//----------------------------------------------------------------------------------------------------------------------------------------

Udouble             LaunchTime = 0.0;   // depuis VIP2Headers.h
USynchronisater*    AllSynchro = NULL;  // depuis VIP2Headers.h
double              TimePreced = 0.0;   // var pour calcul de Fps ( voar utils.h )
U32                 TickNum = 0;    // var pour calcul de Fps ( voar utils.h )
float               m_fps =0.0f;    // var pour calcul de Fps ( voar utils.h )
MP3AudioProvider* prov;
OSSAudioDriver* mp3;

bool    m_Fullscreen = TRUE;
U32 m_Resx = 640;
U32 m_Resy = 480;   // resolution choisie
bool    m_TrueColor = TRUE; // est-ce ke l'on veux du 32 bit
bool    m_Hiddenpart1   = FALSE;    // est-ce ke ya la partie cachée 1 ?
bool    m_Active = FALSE;

GLWindow* glw;

int main(int argc, char** argv) {
    fprintf(stderr,
            "VIP2 by Popsy Team <u2popsy@free.fr> - http://www.popsyteam.org/vip2\n"
            "Linux port (v1.1) by Sesse/Excess <sgunderson@bigfoot.com>\n"
            "http://www.sesse.net/vip2-linux/\n\n");

    // very rudimentary, might be improved in a future release :-)
    int depth = 32, zbuffer = 16;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-640x480") == 0) {
            m_Resx = 640;
            m_Resy = 480;
            continue;
        }
        if (strcmp(argv[i], "-800x600") == 0) {
            m_Resx = 800;
            m_Resy = 600;
            continue;
        }
        if (strcmp(argv[i], "-1024x768") == 0) {
            m_Resx = 1024;
            m_Resy = 768;
            continue;
        }
        if (strcmp(argv[i], "-1152x864") == 0) {
            m_Resx = 1152;
            m_Resy = 864;
            continue;
        }
        if (strcmp(argv[i], "-1280x960") == 0) {
            m_Resx = 1280;
            m_Resy = 960;
            continue;
        }
        if (strcmp(argv[i], "-1280x1024") == 0) {
            m_Resx = 1280;
            m_Resy = 1024;
            continue;
        }
        if (strcmp(argv[i], "-1400x1050") == 0) {
            m_Resx = 1400;
            m_Resy = 1050;
            continue;
        }
        if (strcmp(argv[i], "-1600x1200") == 0) {
            m_Resx = 1600;
            m_Resy = 1200;
            continue;
        }
        if (strcmp(argv[i], "-16") == 0) {
            depth = 16;
            continue;
        }
        if (strcmp(argv[i], "-32") == 0) {
            depth = 32;
            continue;
        }
        if (strcmp(argv[i], "-z16") == 0) {
            zbuffer = 16;
            continue;
        }
        if (strcmp(argv[i], "-z24") == 0) {
            zbuffer = 24;
            continue;
        }
        if (strcmp(argv[i], "-z32") == 0) {
            zbuffer = 32;
            continue;
        }
        if (strcmp(argv[i], "-fullscreen") == 0) {
            m_Fullscreen = true;
            continue;
        }
        if (strcmp(argv[i], "-windowed") == 0) {
            m_Fullscreen = false;
            continue;
        }
        /*
         * about as well hidden as in the original one -- not too hard to find
         * out when you have the sources anyhow;-)
         */
        if (strcmp(argv[i], "-hiddenpart") == 0) {
            m_Hiddenpart1 = TRUE;
            continue;
        }
        fprintf(stderr, "Allowed switches:\n\n"
                "-help - this screen\n"
                "-640x480, -800x600, -1024x768, -1152x864, -1280x960, -1280x1024, -1400x1050, -1600x1200 - set resolution\n"
                "-16, -32 - set bit depth\n"
                "-z16, -z24, -z32 - set Z-buffer depth\n"
                "-windowed, -fullscreen - set window mode\n");
        exit(0);
    }

    UScheduler  MainScheduler;
    glw = new GLWindow("VIP2 on Linux", m_Resx, m_Resy, depth, m_Fullscreen, zbuffer, -1);

    IniEnvironmentU3D3(m_Resx, m_Resy);
    if (!m_TrueColor) TextureManager::SetForceFlag(LOW_TEXTURE);      // demande de ne loader ke des textures 16 bit


    //-------------------------- Ini Timer
    ClockU3D::Initialise();


    //-------------------------- Ini MP3 Manager
#ifndef NOMP3
    prov = new MP3AudioProvider(MP3_PATH);
    mp3 = new OSSAudioDriver(prov);
#endif


    //-------------------------- Ini le synchronisater
    AllSynchro = new USynchronisater;


    //-------------------------- fait les Pre-loading
    FinalPart::PreLoad();
#if !defined( BFINAL_PART  )
    SmilePart::PreLoad();
#if ! defined( BSMILE_PART )
    LettresPart::PreLoad();
    ColonnePart::PreLoad();
    EtoilePart::PreLoad();
#if !defined( BMULTI_PART )
    CouplePart::PreLoad();
#if !defined( BCOUPLE_PART )
    GhostPart::PreLoad();
    CilsPart::PreLoad();
    OeilPart::PreLoad();
#if !defined( BOEIL_PART )
    PlansPart::PreLoad();
    NeuronePart::PreLoad();
#endif
#endif
#endif
#endif
#endif
    BPeloche::Initialise();
    XGrezille::Initialise();


    if (m_Hiddenpart1)
        PlansPart::ActiveHidenPart();

    //-------------------------- Ini la 1ere part de la demo
#ifdef  BFINAL_PART
    FinalPart::Initialise(&MainScheduler, 0);    // commence a partir des scenes final
#elif defined( BSMILE_PART )
    SmilePart::Initialise(&MainScheduler, 0);    // commence a partir des scenes du smile de gonzesse
#elif defined( BMULTI_PART )
    MultiPart::Initialise(&MainScheduler, 0);    // commence a partir des scenes multiples
#elif defined( BCOUPLE_PART )
    CouplePart::Initialise(&MainScheduler, 0);   // commence a partir du couple
#elif defined( BLETTRES_PART )
    LettresPart::Initialise(&MainScheduler, 0);      // commence a partir des lettres
#elif defined( BGHOST_PART )
    GhostPart::Initialise(&MainScheduler, 0);     // commence a partir de la colonne
#elif defined( BCOLONNE_PART )
    ColonnePart::Initialise(&MainScheduler, 0);       // commence a partir de la colonne
#elif defined( BCILS_PART )
    CilsPart::Initialise(&MainScheduler, 0);     // commence a partir des cils
#elif defined( BETOILE_PART )
    EtoilePart::Initialise(&MainScheduler, 0);   // commence a partir de l'etoile
#elif defined( BOEIL_PART )
    OeilPart::Initialise(&MainScheduler, 0);     // commence a partir de l'oeil
#elif defined( BPLANS_PART )
    PlansPart::Initialise(&MainScheduler, 0);    // commence a partir des plans
#elif defined( BNEURO_PART )
    NeuronePart::Initialise(&MainScheduler, 0);      // commence a partir des neurones
#else
    NebuleusePart::Initialise(&MainScheduler, 0);    // commence au debut
#endif


#ifdef NOMP3
    LaunchTime = ClockU3D::GetTime();   // ini temps d'offset
#endif

#ifdef BSMILE_PART
    ClockU3D::IncTime(SMILE_TIME_BEGIN);
#elif defined( BMULTI_PART )
    ClockU3D::IncTime(MULTI_TIME_BEGIN);
#elif defined( BCOUPLE_PART )
    ClockU3D::IncTime(COUPLE_TIME_BEGIN);
#elif defined( BGHOST_PART )
    ClockU3D::IncTime(GHOST_TIME_BEGIN);
#elif defined( BCILS_PART )
    ClockU3D::IncTime(CILS_TIME_BEGIN);
#elif defined( BOEIL_PART )
    ClockU3D::IncTime(OEIL_TIME_BEGIN);
#elif defined( BPLANS_PART )
    ClockU3D::IncTime(PLANS_TIME_BEGIN);
#elif defined( BNEURO_PART )
    ClockU3D::IncTime(NEURO_TIME_BEGIN);
#endif

    //////////////////////////////////////////////////////////////
    //              M A I N     L O O P                         //
    //////////////////////////////////////////////////////////////
    glDepthMask(GL_TRUE);       // permet d'ecrire dans le Zbuffer
    glDepthFunc(GL_LEQUAL);       // met en place le test du Zbuffer
    glEnable(GL_DEPTH_TEST);       // test Zbuff OK

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, m_Resx, m_Resy);
//  glOrtho(0, Xres-1, 0, Yres-1, 1.f/LointainZClipping, 1.f/ProcheZClipping );       // enleve la projection
    glOrtho(0, m_Resx-1, 0, m_Resy-1, 1.f/ProcheZClipping, 1.f/LointainZClipping);        // enleve la projection
    glMatrixMode(GL_MODELVIEW);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);         // met la coretion de perspective
    //
    glLineWidth(1.0f);         // épaisseur de 1 pour une ligne
    glEnable(GL_LINE_SMOOTH);
    //
    //                                                                        //--------------------------- Ini le fog
    glFogi(GL_FOG_MODE, GL_LINEAR);          // equation utilisée pour le fog
    glFogf(GL_FOG_DENSITY, 0.35f);       // Density du fog
    glHint(GL_FOG_HINT, GL_DONT_CARE);        // on veux du fog ki torche
    glFogf(GL_FOG_START, 1.0f);       // debut du fog
    glFogf(GL_FOG_END, 0.7f);       // fin du fog*/
    glDisable(GL_FOG);;       // pas de fog par defaut
    //

    m_Active = TRUE;
    while (!glw->is_done() && ClockU3D::GetTime() < FINAL_TIME_END) {
#ifndef NOMP3
        (void)mp3->run();
        ClockU3D::mp3_timer = mp3->get_time();
#endif
        AllSynchro->Run();  // op gère les gnolies synchrosi
        MainScheduler.Run();    // lance les différentes taches
    }

    //////////////////////////////////////////////////////////////
    //                                                          //
    //////////////////////////////////////////////////////////////


    //------------------------ Stop MP3
#ifndef NOMP3
    delete mp3;
#endif


    //------------------------ Kill toutes les parts koi kil arrive
    XGrezille::Release();
    BPeloche::Release();
    NebuleusePart::Release(NULL, 0);
    NeuronePart::Release(NULL, 0);
    PlansPart::Release(NULL, 0);
    OeilPart::Release(NULL, 0);
    EtoilePart::Release(NULL, 0);
    CilsPart::Release(NULL, 0);
    ColonnePart::Release(NULL, 0);
    GhostPart::Release(NULL, 0);
    LettresPart::Release(NULL, 0);
    CouplePart::Release(NULL, 0);
    MultiPart::Release(NULL, 0);
    SmilePart::Release(NULL, 0);
    FinalPart::Release(NULL, 0);


    //------------------------ Kill toutes les synchros
    DeleteBlinde(AllSynchro);
    delete glw;
    exit(0);
}
//----------------------------------------------------------------------------------------------------------------------------------------
