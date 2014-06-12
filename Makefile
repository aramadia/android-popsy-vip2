OBJS=Sources/BPeloche.o Sources/BSplineAff.o Sources/CilsBack.o   \
	Sources/CilsBoule.o Sources/CilsCarotus.o Sources/Cils.o  \
	Sources/CilsTextes.o Sources/ColonneAnim.o		  \
	Sources/Colonne.o Sources/Couple.o			  \
	Sources/CouplePetals3D.o Sources/CouplePetals.o		  \
	Sources/CouplePluieL.o Sources/CouplePluieP.o		  \
	Sources/CoupleRPluie.o Sources/CoupleTextes.o		  \
	Sources/Etoile.o Sources/EtoileGreets.o \
	Sources/EtoileScale.o Sources/Final.o			  \
	Sources/FinalSpline.o Sources/FinalTexte.o		  \
	Sources/Ghost2D.o Sources/Ghost.o Sources/GhostEclaires.o \
	Sources/GhostEtincelle.o Sources/GhostVPort.o		  \
	Sources/HSplineAff.o Sources/ImageBlur.o Sources/Image.o  \
	Sources/LettresBack.o Sources/Lettres.o			  \
	Sources/Multi.o Sources/Nebuleuse.o  			  \
	Sources/NebuPart.o Sources/NebuTexte.o Sources/NeuroBack.o\
	Sources/NeuroLights.o Sources/NeuroLines.o		  \
	Sources/Neurone.o Sources/NeuroTexte.o			  \
	Sources/ObjDeformation.o Sources/Oeil.o			  \
	Sources/OeilDefor.o Sources/OeilLignes.o		  \
	Sources/OeilTextes.o Sources/PlansAnim.o		  \
	Sources/PlansCache.o Sources/PlansClock.o Sources/Plans.o \
	Sources/PlansTextes.o Sources/PlansTransp.o		  \
	Sources/Scheduler.o Sources/Smile.o	 		  \
	Sources/Synchronisater.o Sources/utils.o		  \
	Sources/XGrezilles.o Sources/linuxcompat.o		  \
	Sources/main.o Sources/glwindow.o Sources/mp3.o		  \
	Sources/linux_oss.o

SOURCES := $(OBJS:.o=.cpp)

U3DOBJS=LIBS/u3dLib/sources/2Dstuff.o LIBS/u3dLib/sources/Bitmap.o  \
	LIBS/u3dLib/sources/Boundings.o LIBS/u3dLib/sources/Bsp.o   \
	LIBS/u3dLib/sources/BSpline.o LIBS/u3dLib/sources/BTree.o   \
	LIBS/u3dLib/sources/CameraConv.o			    \
	LIBS/u3dLib/sources/camera.o LIBS/u3dLib/sources/ClockU3D.o \
	LIBS/u3dLib/sources/DataFile.o LIBS/u3dLib/sources/Emess.o  \
	LIBS/u3dLib/sources/Fpipe.o LIBS/u3dLib/sources/Hide.o      \
	LIBS/u3dLib/sources/LightConv.o LIBS/u3dLib/sources/light.o \
	LIBS/u3dLib/sources/Lignes.o				    \
	LIBS/u3dLib/sources/LmapsManager.o			    \
	LIBS/u3dLib/sources/M44u3d.o LIBS/u3dLib/sources/mat3x4.o   \
	LIBS/u3dLib/sources/material.o				    \
	LIBS/u3dLib/sources/MathMacro.o				    \
	LIBS/u3dLib/sources/objet3D.o				    \
	LIBS/u3dLib/sources/Objet3dWire.o			    \
	LIBS/u3dLib/sources/ObjetBones.o			    \
	LIBS/u3dLib/sources/ObjetConv.o				    \
	LIBS/u3dLib/sources/ObjStaticLighting.o			    \
	LIBS/u3dLib/sources/opengl.o LIBS/u3dLib/sources/OSspec.o   \
	LIBS/u3dLib/sources/Plan.o LIBS/u3dLib/sources/Polygon.o    \
	LIBS/u3dLib/sources/polyprimOpenGL.o			    \
	LIBS/u3dLib/sources/quat.o LIBS/u3dLib/sources/Rasteriz.o   \
	LIBS/u3dLib/sources/Spline.o LIBS/u3dLib/sources/TexMana.o  \
	LIBS/u3dLib/sources/TextureU3D.o LIBS/u3dLib/sources/Txm.o  \
	LIBS/u3dLib/sources/U3d3.o LIBS/u3dLib/sources/umanager.o   \
	LIBS/u3dLib/sources/video.o				    \
	LIBS/u3dLib/sources/World3DConv.o			    \
	LIBS/u3dLib/sources/World3d.o
MPGOBJS=LIBS/mpglib/common.o LIBS/mpglib/dct64_i386.o LIBS/mpglib/decode_i386.o \
	LIBS/mpglib/interface.o LIBS/mpglib/layer3.o		    \
	LIBS/mpglib/tabinit.o

CC=clang
CXX=clang++
FLAGS=-O2
CPPFLAGS=-IInclude -ILIBS/u3dLib/include -ILIBS/mpglib -I/usr/local/Cellar/jpeg/8d/include -I/opt/X11/include
OSXFLAGS=
#-I/usr/local/Cellar/jpeg/8d/include -I/opt/X11/include
CXXFLAGS=$(CFLAGS) $(CPPFLAGS) $(OSXFLAGS)
LDFLAGS=

# CXXCPP="g++ -E" CXX="g++ -arch i386 -arch x86_64" CPPFLAGS="-I/usr/X11/include -I/usr/X11/include/freetype2" LDFLAGS="-L/usr/X11/lib" ./configure --with-x --with-xft --with-opengl --x-includes=/usr/X11/include --x-libraries=/usr/X11/lib

all: vip2

clean:
	$(RM) $(OBJS) $(U3DOBJS) $(MPGOBJS) libu3d.a libmpg.a vip2

libu3d.a: $(U3DOBJS)
	ar rc libu3d.a $(U3DOBJS)
	ranlib libu3d.a

libmpg.a: $(MPGOBJS)
	ar rc libmpg.a $(MPGOBJS)
	ranlib libmpg.a


vip2: $(OBJS) libu3d.a libmpg.a
	$(CXX) -o vip2 $(SOURCES) $(CPPFLAGS) -L. -lu3d -lmpg -framework OpenGL -framework GLUT -ljpeg -L/usr/local/Cellar/jpeg/8d/lib -L/usr/X11R6/lib -lX11 -lXxf86vm.1 -lGL -lGLU

	#mac 	$(CXX) -o vip2 $(SOURCES) $(CPPFLAGS) -L. -lu3d -lmpg -lGL -lGLU -ljpeg -L/usr/X11R6/lib -lXxf86vm

