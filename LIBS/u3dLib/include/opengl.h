/*-----------------------------------------------------+
 |                       OpenGl.h                       |
 |                                                      |
 |      c'est qq fonctions qui me permettent de gérer   |
 |              l'OpenGl dans le moteur tridé héhé      |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _OpenGL_U3D3_H
#define _OpenGL_U3D3_H

#include <GL/gl.h>
#include "glut.h"

#define APIENTRY

class Viewport;   // from Camera.h

//--------------------------- Extention OpenGL pour le multi texturing

typedef void (APIENTRY* PROCMULTITEXCOORD2DFARB)(GLenum target, GLfloat u, GLfloat v);
typedef void (APIENTRY* PROCACTIVETEXTUREARB)(GLenum target);

//extern PROCACTIVETEXTUREARB       glActiveTextureARB;
//extern PROCMULTITEXCOORD2DFARB    glMultiTexCoord2fARB;
//#define glActiveTexture           (*glActiveTextureARB)
//#define glMultiTexCoord2f     (*glMultiTexCoord2fARB)


/* ARB_multitexture */
#define GL_ACTIVE_TEXTURE_ARB               0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB        0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB            0x84E2
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1
#define GL_TEXTURE2_ARB                     0x84C2
#define GL_TEXTURE3_ARB                     0x84C3
#define GL_TEXTURE4_ARB                     0x84C4
#define GL_TEXTURE5_ARB                     0x84C5
#define GL_TEXTURE6_ARB                     0x84C6
#define GL_TEXTURE7_ARB                     0x84C7
#define GL_TEXTURE8_ARB                     0x84C8
#define GL_TEXTURE9_ARB                     0x84C9
#define GL_TEXTURE10_ARB                    0x84CA
#define GL_TEXTURE11_ARB                    0x84CB
#define GL_TEXTURE12_ARB                    0x84CC
#define GL_TEXTURE13_ARB                    0x84CD
#define GL_TEXTURE14_ARB                    0x84CE
#define GL_TEXTURE15_ARB                    0x84CF
#define GL_TEXTURE16_ARB                    0x84D0
#define GL_TEXTURE17_ARB                    0x84D1
#define GL_TEXTURE18_ARB                    0x84D2
#define GL_TEXTURE19_ARB                    0x84D3
#define GL_TEXTURE20_ARB                    0x84D4
#define GL_TEXTURE21_ARB                    0x84D5
#define GL_TEXTURE22_ARB                    0x84D6
#define GL_TEXTURE23_ARB                    0x84D7
#define GL_TEXTURE24_ARB                    0x84D8
#define GL_TEXTURE25_ARB                    0x84D9
#define GL_TEXTURE26_ARB                    0x84DA
#define GL_TEXTURE27_ARB                    0x84DB
#define GL_TEXTURE28_ARB                    0x84DC
#define GL_TEXTURE29_ARB                    0x84DD
#define GL_TEXTURE30_ARB                    0x84DE
#define GL_TEXTURE31_ARB                    0x84DF

/* EXT_texture_env_combine */
#define GL_COMBINE_EXT                      0x8570
#define GL_COMBINE_RGB_EXT                  0x8571
#define GL_COMBINE_ALPHA_EXT                0x8572
#define GL_RGB_SCALE_EXT                    0x8573
#define GL_ADD_SIGNED_EXT                   0x8574
#define GL_INTERPOLATE_EXT                  0x8575
#define GL_CONSTANT_EXT                     0x8576
#define GL_PRIMARY_COLOR_EXT                0x8577
#define GL_PREVIOUS_EXT                     0x8578
#define GL_SOURCE0_RGB_EXT                  0x8580
#define GL_SOURCE1_RGB_EXT                  0x8581
#define GL_SOURCE2_RGB_EXT                  0x8582
#define GL_SOURCE0_ALPHA_EXT                0x8588
#define GL_SOURCE1_ALPHA_EXT                0x8589
#define GL_SOURCE2_ALPHA_EXT                0x858A
#define GL_OPERAND0_RGB_EXT                 0x8590
#define GL_OPERAND1_RGB_EXT                 0x8591
#define GL_OPERAND2_RGB_EXT                 0x8592
#define GL_OPERAND0_ALPHA_EXT               0x8598
#define GL_OPERAND1_ALPHA_EXT               0x8599
#define GL_OPERAND2_ALPHA_EXT               0x859A


//-------------------------- Op la des fonctions bien utiles
//HGLRC EnableOpenGL( HWND hWnd=NULL, HDC dc=NULL, bool HighColor=TRUE, bool HighZbuf=TRUE );   // ini openGL pour moteur 3D ! renvoie le contexte
//void DisableOpenGL( bool real=FALSE );   // si real est a TRUA alors on release pas le DC courant de hWndMain
void EnableFogOpenGL();
void DisableFogOpenGL();
void FlipOpenGL();
void ClearOpenGLZbuffer();
void ClearOpenGLBuffers();   // efface ecran et Zbuffer
void CopyBackToFrontBuffer();
//void SetActualOpenGLContext( HGLRC hglrc, HDC dc );
void SetViewportOpenGL(Viewport& vi);

// va afficher un text aux coordonnées ecrans x,y
void TextOpenGL(GLuint x, GLuint y, GLuint scale, const char* format, ...);

#endif
