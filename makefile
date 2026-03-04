CXX      := g++

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Iinclude $(SDL_CFLAGS)
CXXFLAGS_DEBUG := -std=c++23 -g -Iinclude $(SDL_CFLAGS)
LDFLAGS  := $(SDL_LDFLAGS)

SRCDIR   := src
OBJDIR   := obj
BINDIR   := bin

TARGET   := $(BINDIR)/main.exe
TARGET_DEBUG := $(BINDIR)/main_debug.exe
SOURCES  := $(shell find $(SRCDIR) -name '*.cpp' )
OBJECTS  := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

all: $(BINDIR) $(OBJDIR) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(TARGET_DEBUG)

run: all
	./$(TARGET)

debug-build:
	$(CXX) $(CXXFLAGS_DEBUG) $(SOURCES) -o $(TARGET_DEBUG) $(LDFLAGS)

debug: debug-build
	gdb -batch -ex run -ex backtrace ./$(TARGET_DEBUG)

debug-interactive: debug-build
	gdb ./$(TARGET_DEBUG)

.PHONY: all clean run debug-build debug debug-interactive
