import os

Default('.')

# are we building debug?
CXXFLAGS = ''
if ARGUMENTS.get('debug', 0):
    CXXFLAGS = '-g -DCORONA_DEBUG'

env = Environment(ENV = os.environ, CXXFLAGS = CXXFLAGS)
Export('env')

SConscript('examples/SConscript')
SConscript('src/SConscript')
SConscript('test/SConscript')
