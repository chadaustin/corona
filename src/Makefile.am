SUBDIRS = libungif-4.1.0

INCLUDES = -I $(top_srcdir)/src/libungif-4.1.0

lib_LTLIBRARIES = libcorona.la
include_HEADERS = corona.h

libcorona_la_SOURCES =				\
	Convert.cpp				\
	Corona.cpp				\
	Debug.cpp				\
	Debug.h					\
	DefaultFileSystem.cpp			\
	DefaultFileSystem.h			\
	Open.h					\
	OpenBMP.cpp				\
	OpenGIF.cpp                             \
	OpenJPEG.cpp				\
	OpenPCX.cpp				\
	OpenPNG.cpp				\
	OpenTGA.cpp				\
	Save.h					\
	SavePNG.cpp				\
	SimpleImage.h				\
	Types.h					\
	Utility.h

libcorona_la_LIBADD = \
	libungif-4.1.0/libungif.la

libcorona_la_LDFLAGS = \
	-release $(VERSION) \
	-lpng \
	-ljpeg

libcorona_la_CFLAGS = \
	-Wall
