CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -g
SRCDIR = src
INCDIR = include
BUILDDIR = build
TESTDIR = tests

SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
TARGET = $(BUILDDIR)/main

# Test files (all test sources, exclude main.cpp from lib sources)
TEST_SOURCES = $(wildcard $(TESTDIR)/*/*.cpp)
LIB_SOURCES = $(filter-out $(SRCDIR)/main.cpp, $(SOURCES))

# Convert test sources to object files, handling subdirectories
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%/%.cpp=$(BUILDDIR)/test_%/%.o) \
               $(LIB_SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

TEST_TARGET = $(BUILDDIR)/test_runner


.PHONY: all clean test force-rebuild

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILDDIR)
	$(CXX) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

$(BUILDDIR)/test_%.o: $(TESTDIR)/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

$(BUILDDIR)/test_%/%.o: $(TESTDIR)/%/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Test target
$(TEST_TARGET): $(TEST_OBJECTS) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(TEST_OBJECTS) -lgtest -lgtest_main -lpthread -o $@


$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

test: clean $(TEST_TARGET)
	./$(TEST_TARGET)


run: clean $(TARGET)
	./$(TARGET)

