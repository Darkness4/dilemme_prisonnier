# Makefile Universal by Marc NGUYEN
# Project Name
TARGET ?= $(shell basename $(CURDIR))

# Compiler
CC ?= gcc
CFLAGS ?= -Wall -Wextra -lm -I .

# Linker
LINKER ?= $(CC)
LFLAGS ?= -Wall -I . -lm

# Project structure
SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin
TESTDIR ?= tests

# Get all files based on project structure
SOURCES := $(shell find $(SRCDIR)/ -type f -name '*.c')
SOURCES_TEST := $(shell find $(TESTDIR)/ -type f -name '*.c')
INCLUDES := $(shell find $(SRCDIR)/ -type f -name '*.h')
INCLUDES_TEST := $(shell find $(TESTDIR)/ -type f -name '*.h')
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJECTS_TEST := $(SOURCES_TEST:$(TESTDIR)/%.c=$(OBJDIR)/%.o)
EXECUTABLE_TEST := $(SOURCES_TEST:$(TESTDIR)/%.c=$(BINDIR)/%)
OBJECTS_NOMAIN := $(filter-out obj/main.o,$(OBJECTS))


# Cleaner
rm = rm -rf

executable: $(BINDIR)/$(TARGET)

all: $(BINDIR)/$(TARGET) tests

tests: CFLAGS += -g
tests: $(EXECUTABLE_TEST)

debug: CFLAGS += -g
debug: all

# Link
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(LINKER) $^ $(LFLAGS) -o $@
	@echo "Linking complete!"

$(EXECUTABLE_TEST): $(BINDIR)/% : $(OBJDIR)/%.o $(OBJECTS_NOMAIN)
	@mkdir -p $(shell dirname $@)
	$(LINKER) $^ $(LFLAGS) -o $@
	@echo "Linking complete!"

# Compile
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"

$(OBJECTS_TEST): $(OBJDIR)/%.o : $(TESTDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"

.PHONY: echoes
echoes:
	@echo "OBJECTS :"
	@echo "$(OBJECTS)"
	@echo "OBJECTS_TEST :"
	@echo "$(OBJECTS_TEST)"
	@echo "INCLUDES :"
	@echo "$(INCLUDES)"
	@echo "INCLUDES_TEST :"
	@echo "$(INCLUDES_TEST)"
	@echo "SOURCES :"
	@echo "$(SOURCES)"
	@echo "SOURCES_TEST :"
	@echo "$(SOURCES_TEST)"
	@echo "EXECUTABLE_TEST :"
	@echo "$(EXECUTABLE_TEST)"

.PHONY: clean
clean:
	@$(rm) $(OBJDIR)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) -f $(BINDIR)
	@echo "Executable removed!"