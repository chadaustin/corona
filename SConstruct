import os
import string
import sys

EnsureSConsVersion(0, 12)
SConsignFile()

base_env = Environment(ENV = os.environ)

if os.name is 'nt':
    # MSVC options
    base_env.Append(CXXFLAGS = ['/GX', '/DWIN32', '/D_WIN32',
                                '/DCORONA_EXPORTS', '/Dfor=if(0); else for'])
elif string.find(sys.platform, 'irix') == -1:
    # on IRIX, don't add these options to the build
    base_env.Append(CXXFLAGS = ['-Wall', '-Wno-non-virtual-dtor'])
    # are we building debug?
    if ARGUMENTS.get('debug'):
        base_env.Append(CXXFLAGS = ['-g', '-DCORONA_DEBUG'])
else:
    # but do make sure we look in /usr/freeware for libraries
    base_env.Prepend(CPPPATH = ['/usr/freeware/include'],
                     LIBPATH = ['/usr/freeware/$LIBDIR'])
    if ARGUMENTS.get('debug'):
        base_env.Append(CCFLAGS = ['-g', '-DCORONA_DEBUG'])
    if ARGUMENTS.get('64'):
        base_env.Append(CCFLAGS = ['-64', '-mips4'],
                        LINKFLAGS = ['-64', '-mips4'])
        base_env['LIBDIR'] = 'lib64'
    else:
        base_env['LIBDIR'] = 'lib32'
    

CORONA_LIBS = ['corona', 'png', 'z', 'jpeg']

Export('base_env CORONA_LIBS')

SConscript(dirs = ['examples', 'src', 'test', 'tools'])
