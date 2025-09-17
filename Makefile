CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
SRCDIR = src
INCDIR = include
BUILDDIR = build
TESTDIR = tests

SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TARGET = $(BUILDDIR)/main

# Test files (exclude main.cpp, include calculator.cpp for linking)
TEST_SOURCES = $(wildcard $(TESTDIR)/*.cpp)
LIB_SOURCES = $(filter-out $(SRCDIR)/main.cpp, $(SOURCES))
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.cpp=$(BUILDDIR)/test_%.o) $(LIB_SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TEST_TARGET = $(BUILDDIR)/test_runner

# Validation tool
VALIDATE_TARGET = $(BUILDDIR)/validate_shapes

.PHONY: all clean test run-tests validate force-rebuild

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILDDIR)
	$(CXX) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

$(BUILDDIR)/test_%.o: $(TESTDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Test target
$(TEST_TARGET): $(TEST_OBJECTS) | $(BUILDDIR)
	$(CXX) $(TEST_OBJECTS) -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

test: $(TARGET)
	./$(TARGET)

run-tests: $(TEST_TARGET)
	./$(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

# Validation tool targets
$(VALIDATE_TARGET): $(TESTDIR)/tetris/validate_shapes.cpp $(LIB_SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(TESTDIR)/tetris/validate_shapes.cpp $(LIB_SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o) -o $@

validate: clean $(VALIDATE_TARGET)
	./$(VALIDATE_TARGET)