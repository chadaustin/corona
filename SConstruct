import os
from os import path
pj = path.join


Default('.')


BUILD = 'dist'


VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_PATCH = 0


# XXXaegis hacky test for GNU system
GNU_SYSTEM = 0
print 'attempting to detect a GNU system...'
cmd = 'tar cfz _temp_file.tgz _temp_file && rm _temp_file.tgz'
if os.system('touch _temp_file') == 0 and os.system(cmd) == 0:
    print 'found GNU system!'
    GNU_SYSTEM = 1

try:
    os.remove('_temp_file')
finally:
    pass
    

if not GNU_SYSTEM:
    print 'Warning: GNU system not detected: not building autoconf dist'

env = Environment(ENV = os.environ)
Export('env')


if GNU_SYSTEM:

    TAR_GZ = 'corona-%d.%d.%d.tar.gz' % (
        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH)


    DOCS = [ 'changelog.txt', 'code_guidelines.txt', 'faq.txt',
             'license.txt', 'literature.txt', 'readme.txt',
             'todo.txt', 'tutorial.txt' ]

    SOURCES = [ 'Corona.cpp', 'corona.h', 'Debug.cpp', 'Debug.h',
                'DefaultFileSystem.cpp', 'DefaultFileSystem.h',
                'OpenBMP.cpp', 'Open.h', 'OpenJPEG.cpp',
                'OpenPCX.cpp', 'OpenPNG.cpp', 'OpenTGA.cpp', 'Save.h',
                'SavePNG.cpp', 'SimpleImage.h', 'Types.h', 'Utility.h'
                ]

    BASE  = pj(BUILD, 'autotools')

    INSTALL_TARGETS = []

    for file in DOCS:
        dir = pj(BASE, 'documentation')
        env.Install(dir, pj('documentation', file))
        INSTALL_TARGETS.append(pj(dir, file))
    
    for file in SOURCES:
        dir = pj(BASE, 'src')
        env.Install(dir, pj('src', file))
        INSTALL_TARGETS.append(pj(dir, file))

    AUTOTOOLFILES = [ 'config.guess', 'config.sub', 'depcomp',
                      'install-sh', 'ltmain.sh', 'Makefile.am',
                      'missing', 'mkinstalldirs' ]

    for file in AUTOTOOLFILES:
        env.Install(BASE, pj('autotools', file))
        INSTALL_TARGETS.append(pj(BASE, file))

    env.Install(pj(BASE, 'documentation'), 'autotools/documentation/Makefile.am')
    env.Install(pj(BASE, 'src'),	       'autotools/src/Makefile.am')
    INSTALL_TARGETS.append(pj(BASE, 'documentation', 'Makefile.am'))
    INSTALL_TARGETS.append(pj(BASE, 'src', 'Makefile.am'))


    # stick version information in the configure.in file
    SED_COMMAND = ("sed -e 's/__MAJOR_VERSION__/%d/' " % VERSION_MAJOR) \
                    + ("-e 's/__MINOR_VERSION__/%d/' " % VERSION_MINOR) \
                    + ("-e 's/__PATCH_VERSION__/%d/' " % VERSION_PATCH)

    env.Command(pj(BASE, 'configure.in'),
                pj('autotools', 'configure.in.in'),
                SED_COMMAND + "< $SOURCES > $TARGET")

    env.Command(pj(BASE, 'aclocal.m4'),
                INSTALL_TARGETS + [ pj(BASE, 'configure.in') ],
                "(cd %s && aclocal)" % BASE)

    AUTOMAKE_REQUIRES = AUTOTOOLFILES + [ 'configure.in', 'aclocal.m4' ]
    AUTOMAKE_REQUIRES = map(lambda a: pj(BASE, a), AUTOMAKE_REQUIRES)
    AUTOMAKE_REQUIRES.extend(INSTALL_TARGETS)

    env.Command(pj(BASE, 'Makefile.in'),
                AUTOMAKE_REQUIRES,
                "(cd %s && automake --foreign)" % BASE)

    env.Command(pj(BASE, TAR_GZ),
                pj(BASE, 'configure.in'),
                "(cd %s && autoconf && ./configure && make dist)" % BASE)

    env.Install(BUILD, pj(BASE,  TAR_GZ))

    # end autotools build


# normal SCons builders
SConscript('examples/SConscript')
SConscript('src/SConscript')
SConscript('test/SConscript')
