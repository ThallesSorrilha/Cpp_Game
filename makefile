CXX := g++

MY_LIB := ../my-lib
TINYXML := ../tinyxml2
TINYXML_SRC := $(TINYXML)/tinyxml2.cpp
TINYXML_OBJ = $(OBJDIR)/external/tinyxml2.o
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -MMD -MP -Iinclude -isystem $(MY_LIB)/include -isystem $(TINYXML) $(SDL_CFLAGS)
CXXFLAGS_DEBUG := -std=c++23 -g -MMD -MP -Iinclude -isystem $(MY_LIB)/include -isystem $(TINYXML) $(SDL_CFLAGS)
LDFLAGS  := $(SDL_LDFLAGS) -lSDL2_image

SRCDIR := src
OBJDIR := obj
BINDIR := bin

TARGET := $(BINDIR)/main.exe
TARGET_DEBUG := $(BINDIR)/main_debug.exe
SOURCES := $(shell find $(SRCDIR) -name '*.cpp' )
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

BLUE := \033[1;34m
YELLOW := \033[1;33m
RESET := \033[0m

#-include $(OBJECTS:.o=.d)

all: $(BINDIR) $(OBJDIR) $(TARGET)

$(TARGET): $(OBJECTS) $(TINYXML_OBJ)
	@printf "%b" "$(YELLOW) .o > .exe $(RESET)\n"
	$(CXX) $(OBJECTS) $(TINYXML_OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@printf "%b" "$(BLUE) $< $(RESET)\n"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TINYXML_OBJ): $(TINYXML_SRC)
	@printf "%b" "$(BLUE) $< $(RESET)\n"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(TARGET_DEBUG)

run: all
	@printf "%b" "$(YELLOW) .exe >>> $(RESET)\n"
	./$(TARGET)

debug-build:
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(SOURCES) $(TINYXML_SRC) -o $(TARGET_DEBUG) $(LDFLAGS)

debug: debug-build
	gdb -batch -ex run -ex backtrace ./$(TARGET_DEBUG)

debug-interactive: debug-build
	gdb ./$(TARGET_DEBUG)

.PHONY: all clean run debug-build debug debug-interactive
