import os
import string
import sys

Default('.')

base_env = Environment(ENV = os.environ)

if os.name is 'nt':
    # MSVC options
    base_env.Append(CXXFLAGS = ['/GX', '/DWIN32', '/D_WIN32',
                                '/DCORONA_EXPORTS', '/Dfor=if(0); else for'])
    pass
elif string.find(sys.platform, 'irix') == -1:
    # on IRIX, don't add these options to the build
    # are we building debug?
    base_env.Append(CXXFLAGS = ['-Wall', '-Wno-non-virtual-dtor'])
    if ARGUMENTS.get('debug', 0):
        base_env.Append(CXXFLAGS = ['-g', '-DCORONA_DEBUG'])
else:
    # but do make sure we look in /usr/freeware for libraries
    base_env.Prepend(CPPPATH = ['/usr/freeware/include'],
                LIBPATH = ['/usr/freeware/lib32'])
    

CORONA_LIBS = ['corona', 'png', 'z', 'jpeg']

Export('base_env CORONA_LIBS')

SConscript(dirs = ['examples', 'src', 'test'])
