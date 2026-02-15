CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic -Iinclude

SRCDIR   := src
OBJDIR   := obj
BINDIR   := bin

TARGET   := $(BINDIR)/teste.exe
SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: $(BINDIR) $(OBJDIR) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR) $(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

run: all
	./$(TARGET)

.PHONY: all clean run
