import sys
import os

env = Environment(ENV = {
    'PATH' : os.environ['PATH']
})

env.ParseConfig('pkg-config --cflags glfw3')
env.ParseConfig('pkg-config --libs --static glfw3')
#env['ENV']['TERM'] = os.environ['TERM'] #Adds color to clang output. No idea why it works
env.Append(LIBS = ['glfw3'])
env.Append(CXXFLAGS = ['-std=c++11', '-stdlib=libc++', '-ggdb'])

sources = [
	Glob('./src/*.cpp'),		# Add all cpp files.
]

env.Program(target='./build/gbemuc11', source=sources)