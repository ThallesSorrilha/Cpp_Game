CXX      := g++

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Iinclude $(SDL_CFLAGS) -I/ucrt64/include/box2d
LDFLAGS  := $(SDL_LDFLAGS) -lbox2d

SRCDIR   := src
OBJDIR   := obj
BINDIR   := bin

TARGET   := $(BINDIR)/main.exe
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
	rm -rf $(OBJDIR) $(BINDIR)

run: all
	./$(TARGET)

.PHONY: all clean run
