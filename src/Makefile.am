lib_LTLIBRARIES = libcorona.la
include_HEADERS = corona.h

libcorona_la_SOURCES =				\
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

libcorona_la_LDFLAGS = \
	-release $(VERSION) \
	-lpng \
	-ljpeg

libcorona_la_CFLAGS = \
	-Wall
