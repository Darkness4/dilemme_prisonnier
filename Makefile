# Makefile Universal by Marc NGUYEN
# Project Name
TARGET_CLIENT ?= $(shell basename $(CURDIR))_client
TARGET_SERVER ?= $(shell basename $(CURDIR))_server

# Compiler
CC ?= gcc
CFLAGS ?= -std=gnu11 -pedantic -Wall -Wextra -lpthread -I .

# Linker
LINKER ?= $(CC)
LFLAGS ?= -Wall -I . -lpthread

# Project structure
SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

# Get all files based on project structure
SOURCES_CLIENT := $(shell find $(SRCDIR)/client/ -type f -name '*.c')
SOURCES_SERVER := $(shell find $(SRCDIR)/server/ -type f -name '*.c')

OBJECTS_CLIENT := $(SOURCES_CLIENT:$(SRCDIR)/client/%.c=$(OBJDIR)/client/%.o)
OBJECTS_SERVER := $(SOURCES_SERVER:$(SRCDIR)/server/%.c=$(OBJDIR)/server/%.o)

# Cleaner
rm = rm -rf

all: $(BINDIR)/$(TARGET_CLIENT) $(BINDIR)/$(TARGET_SERVER) cfg

executable: $(BINDIR)/$(TARGET_CLIENT) $(BINDIR)/$(TARGET_SERVER)

debug: CFLAGS += -g
debug: all

cfg: server.properties

server.properties:
	@sh ./scripts/generate-config.sh

# Link
$(BINDIR)/$(TARGET_CLIENT): $(OBJECTS_CLIENT)
	@mkdir -p $(BINDIR)
	$(LINKER) $^ $(LFLAGS) -o $@
	@echo "Linking complete!"

$(BINDIR)/$(TARGET_SERVER):  $(OBJECTS_SERVER)
	@mkdir -p $(BINDIR)
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

.PHONY: echoes
echoes:
	@echo "OBJECTS_CLIENT :"
	@echo "$(OBJECTS_CLIENT)"
	@echo "SOURCES_CLIENT :"
	@echo "$(SOURCES_CLIENT)"
	@echo "OBJECTS_SERVER :"
	@echo "$(OBJECTS_SERVER)"
	@echo "SOURCES_SERVER :"
	@echo "$(SOURCES_SERVER)"

.PHONY: clean
clean:
	@$(rm) $(OBJDIR)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) -f $(BINDIR)
	@echo "Executable removed!"