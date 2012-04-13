    				 /*-----------------------------------------------------+
					  |				         OpenGl.cpp						|
					  |														|
					  |		c'est qq fonctions qui me permettent de gérer   |
					  |				l'OpenGl dans le moteur tridé héhé		|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"
#include <stdarg.h>
#include <glwindow.h>

//----------------------------------------------------------------------------------------------------------
//								+------------------------------+
//								|    Les Variables globales    |
//								+------------------------------+
//----------------------------------------------------------------------------------------------------------

//PROCACTIVETEXTUREARB	glActiveTextureARB		= NULL	;
//PROCMULTITEXCOORD2DFARB glMultiTexCoord2fARB	= NULL	;

//static HDC CurrentOpenglDC;
//static HGLRC CurrentHGLRC;


//----------------------------------------------------------------------------------------------------------
//									+---------------------+
//									|    Les Fonctions    |
//									+---------------------+
//----------------------------------------------------------------------------------------------------------

extern GLWindow *glw;

//----------------------------------------------------------------------------------------------------------
static void GetGLProcs()
{
#if 0
	char *str	;

	if( glActiveTextureARB && glMultiTexCoord2fARB ) return	;

	//---------------------- Test la présence d'extensions indispensables !

	str = (char *) glGetString(GL_EXTENSIONS)	;

	if(!str)
	{
		MessageBox( NULL, "No OpenGL Extention !!!! Arg", "Fuck! Fuck Fuck!", MB_OK )	;
		exit(0)																			;
	}

	if( !strstr( str, "GL_ARB_multitexture" ) )
	{
		MessageBox( NULL, "No OpenGL GL_ARB_multitexture Extention !!!! Arg", "Fuck! Fuck Fuck!", MB_OK )	;
		exit(0)																								;
	}

/*	if( !strstr( str, "EXT_texture_env_combine" ) )
	{
		MessageBox( NULL, "No OpenGL EXT_texture_env_combine Extention !!!! Arg", "Fuck! Fuck Fuck!", MB_OK )	;
		exit(0)																									;
	}*/

	//----------------------


	//---------------------- Récup les pointeurs sur cs fonctions

    glActiveTextureARB = (PROCACTIVETEXTUREARB)wglGetProcAddress("glActiveTextureARB")			;
    glMultiTexCoord2fARB = (PROCMULTITEXCOORD2DFARB)wglGetProcAddress("glMultiTexCoord2fARB")	;

	if( (!glActiveTextureARB) || (!glMultiTexCoord2fARB) )
	{
		MessageBox( NULL, "No OpenGL Multitexturing Extention !!!! Arg", "Fuck! Fuck Fuck!", MB_OK )	;
		exit(0)																							;
	}

	//----------------------
#endif
}
//----------------------------------------------------------------------------------------------------------
#if 0
HGLRC EnableOpenGL( HWND hWnd, HDC dc, bool HighColor, bool HighZbuf )
{
	PIXELFORMATDESCRIPTOR	pfd		;
	int						iFormat	;


	// get the device context (DC)
	if( hWnd )
		CurrentOpenglDC = dc	;
	else
		CurrentOpenglDC = GetDC( hWndMain )	;

	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) )						;
	pfd.nSize = sizeof( pfd )								;
	pfd.nVersion = 1										;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER				;   //  | PFD_DRAW_TO_BITMAP
	pfd.iPixelType = PFD_TYPE_RGBA							;
	if( HighColor )
	{
		pfd.cColorBits = 32	;
	}
	else
	{
		pfd.cColorBits = 16	;
	}
	if( HighZbuf )
	{
		pfd.cDepthBits = 32	;
	}
	else
	{
		pfd.cDepthBits = 16	;
	}
	pfd.iLayerType = PFD_MAIN_PLANE							;
	iFormat = ChoosePixelFormat( CurrentOpenglDC, &pfd )	;
	SetPixelFormat( CurrentOpenglDC, iFormat, &pfd )		;

	// create and enable the render context (RC)
	CurrentHGLRC = wglCreateContext( CurrentOpenglDC )	;
	wglMakeCurrent( CurrentOpenglDC, CurrentHGLRC )		;

	GetGLProcs()	;		// recupere certaines extention OpenGL

	glDepthMask(GL_TRUE)	;	// permet d'ecrire dans le Zbuffer
	glDepthFunc(GL_LEQUAL)	;	// met en place le test du Zbuffer
	glEnable(GL_DEPTH_TEST)	;	// test Zbuff OK

    glMatrixMode(GL_PROJECTION)													;
    glLoadIdentity()															;
//    glOrtho(0, Xres-1, 0, Yres-1, 1.f/LointainZClipping, 1.f/ProcheZClipping )	;	// enleve la projection
    glOrtho(0, Xres-1, 0, Yres-1, 1.f/ProcheZClipping, 1.f/LointainZClipping )	;	// enleve la projection
    glMatrixMode(GL_MODELVIEW)													;

	glHint( GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST )	;	// met la coretion de perspective

	glLineWidth( 1.0f )			;	// épaisseur de 1 pour une ligne
	glEnable( GL_LINE_SMOOTH )	;

	//--------------------------- Ini le fog
	glFogi( GL_FOG_MODE, GL_LINEAR  )				;	// equation utilisée pour le fog
//	glFogfv(GL_FOG_COLOR, fogColor);			;	// couleur du fog
	glFogf(GL_FOG_DENSITY, 0.35f)				;	// Density du fog
	glHint(GL_FOG_HINT, GL_DONT_CARE )				;	// on veux du fog ki torche
	glFogf(GL_FOG_START, 1.0f)	;	// debut du fog
	glFogf(GL_FOG_END, 0.7f)				;	// fin du fog*/
	glDisable(GL_FOG);							;	// pas de fog par defaut

	return CurrentHGLRC	;
}
//----------------------------------------------------------------------------------------------------------
void DisableOpenGL( bool real )
{
	if( real )
	{
		wglMakeCurrent( NULL, NULL )			;
		wglDeleteContext( CurrentHGLRC )		;
	}
	else
	{
		wglMakeCurrent( NULL, NULL )			;
		wglDeleteContext( CurrentHGLRC )		;
		ReleaseDC( hWndMain, CurrentOpenglDC )	;
	}

	glActiveTextureARB = NULL	;
	glMultiTexCoord2fARB = NULL	;
}
#endif
//----------------------------------------------------------------------------------------------------------
void EnableFogOpenGL()
{
	glEnable(GL_FOG)	;
}
//----------------------------------------------------------------------------------------------------------
void DisableFogOpenGL()
{
	glDisable(GL_FOG)	;
}
//----------------------------------------------------------------------------------------------------------
// swap le back et front buffer si yen a 1 poil de sein !
void FlipOpenGL()
{
    glFinish()						;
	glw->flip();
}
//----------------------------------------------------------------------------------------------------------
// copie le back vers le front buffer
void CopyBackToFrontBuffer()
{
	glCopyPixels( 0, 0, Xres-1, Yres-1, GL_COLOR )	;
}
//----------------------------------------------------------------------------------------------------------
void ClearOpenGLZbuffer()
{
	glClearDepth( 1.0f )			;
	glClear( GL_DEPTH_BUFFER_BIT )	;	// clear uniquement Zbuffer
}
//----------------------------------------------------------------------------------------------------------
// efface ecran et Zbuffer
void ClearOpenGLBuffers()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0F)				;
	glClearDepth( 1.0f )								;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT )	;
}
//----------------------------------------------------------------------------------------------------------
#if 0
void SetActualOpenGLContext( HGLRC hglrc, HDC dc )
{
	CurrentHGLRC = hglrc								;
	CurrentOpenglDC	= dc								;
	wglMakeCurrent( CurrentOpenglDC, CurrentHGLRC )		;
}
#endif
//----------------------------------------------------------------------------------------------------------
void SetViewportOpenGL( Viewport &vi )
{
	glViewport( (GLint)vi.GetMinX(), (GLint)vi.GetMinY(), (GLint)vi.GetDx(), (GLint)vi.GetDy() )	;
}
//----------------------------------------------------------------------------------------------------------
#if 0
// va afficher un text aux coordonnées ecrans x,y
void TextOpenGL(GLuint x, GLuint y, GLuint scale, char* format, ...)
{
    va_list args							;
    char	buffer[255], *p					;
    GLfloat font_scale = 119.05F + 33.33F	;

    va_start(args, format)			;
    vsprintf(buffer, format, args)	;
    va_end(args)					;

    glMatrixMode(GL_PROJECTION)		;
    glPushMatrix()					;
    glLoadIdentity()				;
    gluOrtho2D(0, Xres, 0, Yres)	;

    glMatrixMode(GL_MODELVIEW)	;
    glPushMatrix()				;
    glLoadIdentity()			;

    glPushAttrib(GL_ENABLE_BIT)						;
    glDisable(GL_LIGHTING)							;
    glDisable(GL_TEXTURE_2D)						;
    glDisable(GL_DEPTH_TEST)						;
    glTranslatef((GLfloat) x, (GLfloat) y, 0.0F)	;

    glScalef(scale/font_scale, scale/font_scale, scale/font_scale)	;

    for(p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p)	;
 
    glPopAttrib()	;

    glPopMatrix()					;
    glMatrixMode(GL_PROJECTION)		;
    glPopMatrix()					;
    glMatrixMode(GL_MODELVIEW)		;
}
#endif
//----------------------------------------------------------------------------------------------------------
