import os
from os import path
pj = path.join


BUILD = 'dist'
Default(BUILD)


VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_PATCH = 0
TAR_GZ = 'corona-%d.%d.%d.tar.gz' % (
    VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH)


DOCS = [ 'changelog.txt', 'code_guidelines.txt', 'faq.txt',
         'license.txt', 'literature.txt', 'readme.txt', 'todo.txt',
         'tutorial.txt' ]

SOURCES = [ 'Corona.cpp', 'corona.h', 'Debug.cpp', 'Debug.h',
            'DefaultFileSystem.cpp', 'DefaultFileSystem.h',
            'OpenBMP.cpp', 'Open.h', 'OpenJPEG.cpp', 'OpenPCX.cpp',
            'OpenPNG.cpp', 'Save.h', 'SavePNG.cpp', 'SimpleImage.h',
            'Types.h', 'Utility.h' ]

BASE  = pj(BUILD, 'autotools')

INSTALL_TARGETS = []

env = Environment(ENV = os.environ)
Export('env')

for file in DOCS:
    env.Install(BASE + 'documentation', pj('documentation', file))
    INSTALL_TARGETS.append(pj(BASE, 'documentation', file))
    
for file in SOURCES:
    env.Install(BASE + 'src', pj('src', file))
    INSTALL_TARGETS.append(pj(BASE, 'src', file))

AUTOTOOLFILES = [ 'config.guess', 'config.sub', 'install-sh',
                  'ltmain.sh', 'Makefile.am', 'missing',
                  'mkinstalldirs' ]

for file in AUTOTOOLFILES:
    env.Install(BASE, pj('autotools', file))
    INSTALL_TARGETS.append(pj(BASE, file))

env.Install(pj(BASE, 'documentation'), 'autotools/documentation/Makefile.am')
env.Install(pj(BASE, 'src'),           'autotools/src/Makefile.am')
INSTALL_TARGETS.append(pj(BASE, 'documentation', 'Makefile.am'))
INSTALL_TARGETS.append(pj(BASE, 'src', 'Makefile.am'))


SED_COMMAND = ("sed -e 's/__MAJOR_VERSION__/%d/' " % VERSION_MAJOR) \
                + ("-e 's/__MINOR_VERSION__/%d/' " % VERSION_MINOR) \
                + ("-e 's/__PATCH_VERSION__/%d/' " % VERSION_PATCH)

env.Command(pj(BASE, 'configure.in'),
            pj('autotools', 'configure.in.in'),
            SED_COMMAND + "< $SOURCES > $TARGET")

env.Command(pj(BASE, 'aclocal.m4'),
            pj(BASE, 'configure.in'),
            "(cd %s && aclocal)" % BASE)

AUTOMAKE_REQUIRES = AUTOTOOLFILES + [ 'configure.in' ]
AUTOMAKE_REQUIRES = map(lambda a: pj(BASE, a), AUTOMAKE_REQUIRES)
AUTOMAKE_REQUIRES.extend(INSTALL_TARGETS)

env.Command(pj(BASE, 'Makefile.in'),
            AUTOMAKE_REQUIRES,
            "(cd %s && automake --foreign)" % BASE)

env.Command(pj(BASE, TAR_GZ),
            pj(BASE, 'configure.in'),
            "(cd %s && autoconf && ./configure && make dist)" % BASE)

env.Install(BUILD, pj(BASE,  TAR_GZ))



LIBRARY = pj(BUILD, 'library')
BuildDir(LIBRARY, 'src', 0)

SConscript(pj(LIBRARY, 'SConscript'))


