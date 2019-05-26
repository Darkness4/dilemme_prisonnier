# Makefile Universal by Marc NGUYEN
# Project Name
TARGET_CLIENT ?= $(shell basename $(CURDIR))_client
TARGET_SERVER ?= $(shell basename $(CURDIR))_server

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
SOURCES_CLIENT := $(shell find $(SRCDIR)/client/ -type f -name '*.c')
SOURCES_SERVER := $(shell find $(SRCDIR)/server/ -type f -name '*.c')
SOURCES_TEST := $(shell find $(TESTDIR)/ -type f -name '*.c')

INCLUDES_CLIENT := $(shell find $(SRCDIR)/client/ -type f -name '*.h')
INCLUDES_SERVER := $(shell find $(SRCDIR)/server/ -type f -name '*.h')
INCLUDES_TEST := $(shell find $(TESTDIR)/ -type f -name '*.h')

OBJECTS_CLIENT := $(SOURCES_CLIENT:$(SRCDIR)/client/%.c=$(OBJDIR)/client/%.o)
OBJECTS_SERVER := $(SOURCES_SERVER:$(SRCDIR)/server/%.c=$(OBJDIR)/server/%.o)
OBJECTS_TEST := $(SOURCES_TEST:$(TESTDIR)/%.c=$(OBJDIR)/%.o)

EXECUTABLE_TEST := $(SOURCES_TEST:$(TESTDIR)/%.c=$(BINDIR)/%)


# Cleaner
rm = rm -rf

executable: $(BINDIR)/$(TARGET_CLIENT) $(BINDIR)/$(TARGET_SERVER)

all: $(BINDIR)/$(TARGET_CLIENT) $(BINDIR)/$(TARGET_SERVER) tests

tests: CFLAGS += -g
tests: $(EXECUTABLE_TEST)

debug: CFLAGS += -g
debug: all

# Link
$(BINDIR)/$(TARGET_CLIENT): $(OBJECTS_CLIENT)
	@mkdir -p $(BINDIR)
	$(LINKER) $^ $(LFLAGS) -o $@
	@echo "Linking complete!"

$(BINDIR)/$(TARGET_SERVER):  $(OBJECTS_SERVER)
	@mkdir -p $(BINDIR)
	$(LINKER) $^ $(LFLAGS) -o $@
	@echo "Linking complete!"

$(EXECUTABLE_TEST): $(BINDIR)/% : $(OBJDIR)/%.o
	@mkdir -p $(shell dirname $@)
	$(LINKER) $^ $(LFLAGS) -o $@
	@echo "Linking complete!"

# Compile
$(OBJECTS_CLIENT): $(OBJDIR)/client/%.o : $(SRCDIR)/client/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"

$(OBJECTS_SERVER): $(OBJDIR)/server/%.o : $(SRCDIR)/server/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfullly!"

$(OBJECTS_TEST): $(OBJDIR)/%.o : $(TESTDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"

.PHONY: echoes
echoes:
	@echo "OBJECTS_CLIENT :"
	@echo "$(OBJECTS_CLIENT)"
	@echo "INCLUDES_CLIENT :"
	@echo "$(INCLUDES_CLIENT)"
	@echo "SOURCES_CLIENT :"
	@echo "$(SOURCES_CLIENT)"
	@echo "OBJECTS_SERVER :"
	@echo "$(OBJECTS_SERVER)"
	@echo "INCLUDES_SERVER :"
	@echo "$(INCLUDES_SERVER)"
	@echo "SOURCES_SERVER :"
	@echo "$(SOURCES_SERVER)"
	@echo "OBJECTS_TEST :"
	@echo "$(OBJECTS_TEST)"
	@echo "INCLUDES_TEST :"
	@echo "$(INCLUDES_TEST)"
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