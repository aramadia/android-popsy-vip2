#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <GL/glx.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glwindow.h"

#define DEMOLIB_XASPECT 4.0f
#define DEMOLIB_YASPECT 3.0f

void GLWindow::resize(int x, int y, int w, int h) {
    /* Prevent division by zero */
    if (h == 0) {
        h = 1;
    }

    float aspect = (float)w / (float)h;
    if (aspect > DEMOLIB_XASPECT / DEMOLIB_YASPECT) {
        int new_w = (int)((float)h * DEMOLIB_XASPECT / DEMOLIB_YASPECT);
        x += (w - new_w) / 2;
        w = new_w;
    } else if (aspect < DEMOLIB_XASPECT / DEMOLIB_YASPECT) {
        int new_h = (int)((float)w * DEMOLIB_YASPECT / DEMOLIB_XASPECT);
        y += (h - new_h) / 2;
        h = new_h;
    }

    glViewport(x, y, w, h);
}

GLWindow::GLWindow(const char* title, int width, int height, int bpp, bool fullscreen, int zbuffer, int visual_id) {
    XVisualInfo* vi;
    int dpyWidth = 0, dpyHeight = 0;
    int i;
    XF86VidModeModeInfo** modes;
    int modeNum;
    int bestMode;
    Atom wmDelete;
    Window winDummy;
    unsigned int borderDummy;

    static int attrList[] = {
        GLX_RGBA,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE, zbuffer,
        None
    };

    this->x = 0;
    this->y = 0;
    this->width = width;
    this->height = height;
    this->bpp = bpp;
    this->fullscreen = fullscreen;
    this->zbuffer = zbuffer;
    this->done = 0;

    /* set best mode to current */
    bestMode = 0;

    /* get a connection */
    this->dpy = XOpenDisplay(0);
    this->screen = DefaultScreen(this->dpy);

    if (fullscreen) {
        XF86VidModeGetAllModeLines(this->dpy, this->screen, &modeNum, &modes);

        /* save desktop-resolution before switching modes */
        this->deskMode = *modes[0];

        /* look for mode with requested resolution */
        for (i = 0; i < modeNum; i++) {
            if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height)) {
                bestMode = i;
            }
        }

        /* if we don't have it, bomb out */
        if (bestMode == 0 && (modes[0]->hdisplay != width || modes[0]->vdisplay != height)) {
            fprintf(stderr, "Couldn't set requested screen mode.\n");
            exit(-1);
        }
    }

    if (visual_id != -1) {
        XVisualInfo tmplate;
        int nret;

        tmplate.visualid = visual_id;
        vi = XGetVisualInfo(this->dpy, VisualIDMask, &tmplate, &nret);
        if (vi == NULL) {
            fprintf(stderr, "Couldn't get selected visual!\n");
            exit(-1);
        }
    } else {
        /* get an appropriate visual */
        vi = glXChooseVisual(this->dpy, this->screen, attrList);
        if (vi == NULL) {
            fprintf(stderr, "Couldn't get double-buffered visual\n");
            exit(-1);
        }
    }

    /* create a GLX context */
    this->ctx = glXCreateContext(this->dpy, vi, NULL, GL_TRUE);

    /* create a color map (umm, needed?) */
    Colormap cmap = XCreateColormap(this->dpy, RootWindow(this->dpy, vi->screen),
                                    vi->visual, AllocNone);
    this->attr.colormap = cmap;

    /* make a blank cursor */
    {
        static char data[1] = {0};
        Cursor cursor;
        Pixmap blank;
        XColor dummy;

        blank = XCreateBitmapFromData(this->dpy, RootWindow(this->dpy, vi->screen), data, 1, 1);
        if (blank == None) {
            fprintf(stderr, "Out of memory!\n");
            exit(-1);
        }
        cursor = XCreatePixmapCursor(this->dpy, blank, blank, &dummy, &dummy, 0, 0);
        XFreePixmap(this->dpy, blank);
        this->attr.cursor = cursor;
    }

    this->attr.border_pixel = 0;

    /* change screen mode */
    if (fullscreen) {
        XF86VidModeSwitchToMode(this->dpy, this->screen, modes[bestMode]);
        XF86VidModeSetViewPort(this->dpy, this->screen, 0, 0);
        dpyWidth = modes[bestMode]->hdisplay;
        dpyHeight = modes[bestMode]->vdisplay;
        XFree(modes);
    }

    /* create the window */
    this->attr.background_pixel = 0;

    if (fullscreen) {
        /* create a fullscreen window */
        this->attr.override_redirect = True;
        this->attr.event_mask = KeyPressMask | ButtonPressMask |
                                StructureNotifyMask;

        this->win = XCreateWindow(this->dpy, RootWindow(this->dpy, vi->screen),
                                  0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
                                  CWColormap | CWCursor | CWEventMask | CWOverrideRedirect,
                                  &this->attr);
        XWarpPointer(this->dpy, None, this->win, 0, 0, 0, 0, 0, 0);
        XMapRaised(this->dpy, this->win);
        XGrabKeyboard(this->dpy, this->win, True, GrabModeAsync,
                      GrabModeAsync, CurrentTime);
        XGrabPointer(this->dpy, this->win, True, ButtonPressMask,
                     GrabModeAsync, GrabModeAsync, this->win, None, CurrentTime);
    } else {
        /* create a window in window mode*/
        this->attr.event_mask = KeyPressMask | ButtonPressMask |
                                StructureNotifyMask;
        this->win = XCreateWindow(this->dpy, RootWindow(this->dpy, vi->screen),
                                  0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
                                  CWColormap | CWBorderPixel | CWEventMask, &this->attr);

        /* only set window title and handle wm_delete_events if in windowed mode */
        wmDelete = XInternAtom(this->dpy, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(this->dpy, this->win, &wmDelete, 1);
        XSetStandardProperties(this->dpy, this->win, title,
                               title, None, NULL, 0, NULL);
        XMapRaised(this->dpy, this->win);
    }

    /* connect the glx-context to the window */
    glXMakeCurrent(this->dpy, this->win, this->ctx);
    XClearWindow(this->dpy, this->win);
    XGetGeometry(this->dpy, this->win, &winDummy, &this->x, &this->y,
                 &this->width, &this->height, &borderDummy, &this->bpp);
    if (!glXIsDirect(this->dpy, this->ctx)) {
        fprintf(stderr, "No direct rendering (hardware acceleration) available! (Check libGL.so symlinks)\n");
        exit(-1);
    }

    nice(-7);

    this->resize(0, 0, this->width, this->height);
}

GLWindow::~GLWindow() {
    if (this->ctx) {
        if (!glXMakeCurrent(this->dpy, None, NULL)) {
            fprintf(stderr, "Could not release drawing context.\n");
            exit(-1);
        }
        glXDestroyContext(this->dpy, this->ctx);
        this->ctx = NULL;
    }

    if (fullscreen) {
        XF86VidModeSwitchToMode(this->dpy, this->screen, &this->deskMode);
        XF86VidModeSetViewPort(this->dpy, this->screen, 0, 0);
    }

    XCloseDisplay(this->dpy);
}

void GLWindow::flip() {
    while (XPending(this->dpy) > 0) {
        XEvent event;
        XNextEvent(this->dpy, &event);

        switch (event.type) {
        case ButtonPress:
            this->done = true;
            break;
        case KeyPress:
            if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
                this->done = true;
            break;
        }
    }

    glXSwapBuffers(this->dpy, this->win);
}

bool GLWindow::is_done() {
    return this->done;
}
