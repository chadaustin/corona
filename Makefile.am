EXTRA_DIST = documentation

lib_LTLIBRARIES = libcorona.la
include_HEADERS = corona.h

libcorona_la_SOURCES = \
	Corona.cpp \
	DefaultFileSystem.cpp \
	DefaultFileSystem.h \
	Open.h \
	OpenBMP.cpp \
	OpenJPEG.cpp \
	OpenPCX.cpp \
	OpenPNG.cpp \
	Save.h \
	SavePNG.cpp \
	SimpleImage.h \
	Utility.h

libcorona_la_LDFLAGS = \
	-release $(VERSION) \
	-lpng \
	-ljpeg

libcorona_la_CFLAGS = \
	-Wall
