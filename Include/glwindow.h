#ifndef _GLWINDOW_H
#define _GLWINDOW_H

#ifdef WIN32
#include <windows.h>
#endif

#if defined(__linux__) || defined(__APPLE__)
#include <GL/glx.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif


class PBuffer_RTT;

class GLWindow {
public:
    GLWindow(const char* title, int width, int height, int bpp, bool fullscreen, int zbuffer, int visual_id);
    ~GLWindow();
    void resize(int x, int y, int w, int h);
    void flip();
    bool is_done();

    friend class PBuffer_RTT;

protected:
#ifdef WIN32
    HDC hDC;
    HGLRC hRC;
    HWND hWnd;
    HINSTANCE hInstance;
#endif
#if defined(__linux__) || defined(__APPLE__)
    Display* dpy;
    int screen;
    Window win;
    GLXContext ctx;
    XSetWindowAttributes attr;
    Bool fs;
    XF86VidModeModeInfo deskMode;
#endif

    char* title;
    bool fullscreen;
    int x, y;
    unsigned int width, height;
    unsigned int bpp;
    int zbuffer;
    bool done;
    void initGL();
};

#endif
