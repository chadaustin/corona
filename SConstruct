import os

Default('.')

# are we building debug?
CXXFLAGS = ''
if ARGUMENTS.get('debug', 0):
    CXXFLAGS = '-g -DCORONA_DEBUG'

CORONA_LIBS = ['corona', 'png', 'z', 'jpeg', 'ungif']
env = Environment(ENV = os.environ, CXXFLAGS = CXXFLAGS)

Export('env CORONA_LIBS')

SConscript('examples/SConscript')
SConscript('src/SConscript')
SConscript('test/SConscript')
