build_osx:
COMPILER = clang
SOURCE_LIBS = -Ilib/
OSX_OPT = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
OSX_OUT = -o "breakout"
CFILES = src/*.c

build_osx:
	$(COMPILER) -g $(CFILES) $(SOURCE_LIBS) $(OSX_OPT) $(OSX_OUT)