import os
import sys

Default('.')

# are we building debug?
CXXFLAGS = ['-Wall', '-Wno-non-virtual-dtor']
if ARGUMENTS.get('debug', 0):
    CXXFLAGS.extend(['-g', '-DCORONA_DEBUG'])

CORONA_LIBS = ['corona', 'png', 'z', 'jpeg']
env = Environment(ENV = os.environ,
                  CC = 'gcc',
                  CXX = 'g++',
                  CXXFLAGS = CXXFLAGS)

if sys.platform == 'cygwin':
    env.Append(CCFLAGS = ['-DWIN32'],
               CXXFLAGS = ['-DWIN32'])

Export('env CORONA_LIBS')

SConscript(dirs = ['examples', 'src', 'test'])
