CXX := g++
CXXFLAGS := -std=c++20 -O0
CPPFLAGS := -I./include
SDL2_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL2_LIBS   := $(shell pkg-config --libs sdl2)


SRCS := main.cpp $(wildcard src/*/*.cpp)
SDL2_SRCS := sdl2.cpp $(wildcard src/*/*.cpp)

all:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SDL2_CFLAGS) \
	       $(SRCS) \
	       $(SDL2_LIBS) \
	       -o renderer.out

sdl:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SDL2_CFLAGS) \
	       $(SDL2_SRCS) \
	       $(SDL2_LIBS) \
	       -o renderer.out