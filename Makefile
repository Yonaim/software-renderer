CXX := g++
CXXFLAGS := -std=c++23 -O0 -MMD -MP
CPPFLAGS := -I./include -I./defs -I./external
SDL2_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL2_LIBS   := $(shell pkg-config --libs sdl2)

SRCS := $(wildcard src/*/*.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

TARGET := renderer.out
TEST_TARGET := test.out

all: $(TARGET)

$(TARGET): main.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SDL2_CFLAGS) $^ $(SDL2_LIBS) -o $@

test_asset: test/asset.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SDL2_CFLAGS) $^ $(SDL2_LIBS) -o $(TEST_TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SDL2_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(SDL2_OBJS) main.o test/asset.o $(TARGET) $(TEST_TARGET)

re:
	make clean
	make

-include $(DEPS)
