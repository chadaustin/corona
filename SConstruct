import os

Default('.')

sources = [
    'Corona.cpp',
    'Debug.cpp',
    'DefaultFileSystem.cpp',
    'OpenBMP.cpp',
    'OpenJPEG.cpp',
    'OpenPCX.cpp',
    'OpenPNG.cpp',
    'SavePNG.cpp',
]

env = Environment(ENV = os.environ)
env.Library(target = 'corona', source = sources)

SConscript('examples/SConscript')
SConscript('test/SConscript')
