import os

Default('.')

# are we building debug?
CXXFLAGS = ''
if ARGUMENTS.get('debug', 0):
    CXXFLAGS = '-g -DCORONA_DEBUG'

CORONA_LIBS = ['corona', 'png', 'z', 'jpeg']
env = Environment(ENV = os.environ,
                  CC = 'gcc',
                  CXX = 'g++',
                  CXXFLAGS = CXXFLAGS)

Export('env CORONA_LIBS')

SConscript(dirs = ['examples', 'src', 'test'])
