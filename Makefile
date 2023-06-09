Libdir = irrlicht-1.8.5
Sources = ./src/main.cpp
SUF = .o
# general compiler settings
CPPFLAGS = -I$(Libdir)/include -I/usr/X11R6/include
CXXFLAGS = -O3 -ffast-math
#CXXFLAGS = -g -Wall

#default target is Linux
all: all_linux

ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif

# target specific settings
all_linux: LDFLAGS = -L/usr/X11R6/lib$(LIBSELECT) -L$(Libdir)/lib/Linux -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lXcursor
all_linux clean_linux: SYSTEM=Linux
all_win32: LDFLAGS = -L$(Libdir)/lib/Win32-gcc -lIrrlicht -lopengl32 -lm
all_win32: CPPFLAGS += -D__GNUWIN32__ -D_WIN32 -DWIN32 -D_WINDOWS -D_MBCS -D_USRDLL
all_win32 clean_win32: SYSTEM=Win32-gcc
all_win32 clean_win32: SUF=.exe
# name of the binary - only valid for targets which set SYSTEM
DESTPATH = $/bin/game$(SUF)

all_linux all_win32:
	$(warning Building...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(Sources) -o $(DESTPATH) $(LDFLAGS)

clean: clean_linux clean_win32
	$(warning Cleaning...)

clean_linux clean_win32:
	@$(RM) $(DESTPATH)

.PHONY: all all_win32 clean clean_linux clean_win32


# VERSION=1.8.5
#
# gamemake: src/main.cpp
#	g++ -I./irrlicht-$(VERSION)/include -I/usr/X11R6/include  -O3 -ffast-math src/main.cpp -o bin/game.o -L./irrlicht-$(VERSION)/lib/Linux -lIrrlicht -L/usr/X 11R6/lib -lGL -lXxf86vm -lXext -lX11 -lXcursor
