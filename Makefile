# Raylib Starter Pack Makefile
# Adapted from Presto Framework

# Compiler and flags
CC = gcc

# Try to detect raylib using pkg-config first
RAYLIB_PKG := $(shell pkg-config --exists raylib 2>/dev/null && echo "yes" || echo "no")

ifeq ($(RAYLIB_PKG),yes)
    # Use pkg-config if available
    RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib)
    RAYLIB_LDFLAGS := $(shell pkg-config --libs raylib)
else
    # Fallback: try common installation paths
    RAYLIB_PATHS := /usr/local/include /usr/include /opt/raylib/include ~/raylib/src ./raylib/src
    RAYLIB_LIB_PATHS := /usr/local/lib /usr/lib /opt/raylib/lib ~/raylib/src ./raylib/src
    
    # Find raylib header and library (prefer same directory)
    RAYLIB_FOUND_PATH := $(shell for path in $(RAYLIB_PATHS); do \
        if [ -f "$$path/raylib.h" ]; then \
            echo "$$path"; \
            break; \
        fi \
    done)
    
    # Set include path
    RAYLIB_INCLUDE := $(if $(RAYLIB_FOUND_PATH),-I$(RAYLIB_FOUND_PATH),)
    
    # Try to find library in the same directory as header first, then in standard lib paths
    RAYLIB_LIBDIR := $(shell \
        if [ -n "$(RAYLIB_FOUND_PATH)" ] && [ -f "$(RAYLIB_FOUND_PATH)/libraylib.a" -o -f "$(RAYLIB_FOUND_PATH)/libraylib.so" ]; then \
            echo "-L$(RAYLIB_FOUND_PATH)"; \
        else \
            for path in $(RAYLIB_LIB_PATHS); do \
                if [ -f "$$path/libraylib.a" ] || [ -f "$$path/libraylib.so" ]; then \
                    echo "-L$$path"; \
                    break; \
                fi \
            done; \
        fi)
    
    RAYLIB_CFLAGS := $(RAYLIB_INCLUDE)
    RAYLIB_LDFLAGS := $(RAYLIB_LIBDIR) -lraylib -lm -lpthread -ldl -lrt -lX11
endif

# Final compiler flags
CFLAGS = -Wall -Wextra -std=c2x -O2 $(RAYLIB_CFLAGS) -Iinc -Isrc
DEBUG_CFLAGS = -Wall -Wextra -std=c2x -g -DDEBUG $(RAYLIB_CFLAGS) -Iinc -Isrc
LDFLAGS = $(RAYLIB_LDFLAGS)

# Directories
SRCDIR = src
INCDIR = inc
OBJDIR = obj
BINDIR = bin
RESDIR = res

# Source files - automatically find all .c files in src and subdirectories
MAIN_SRC = $(SRCDIR)/main.c
OTHER_SRCS = $(filter-out $(MAIN_SRC), $(shell find $(SRCDIR) -name "*.c" 2>/dev/null))
ALL_SRCS = $(MAIN_SRC) $(OTHER_SRCS)

# Object files
MAIN_OBJ = $(OBJDIR)/main.o
OTHER_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(OTHER_SRCS))
ALL_OBJS = $(MAIN_OBJ) $(OTHER_OBJS)

# Output targets
MAIN_OUT = $(BINDIR)/game
DEBUG_OUT = $(BINDIR)/game-debug

# Windows cross-build output
WINDOWS_OUT = $(BINDIR)/game.exe

# Check if raylib is available
check-raylib:
	@echo "Checking for raylib..."
ifeq ($(RAYLIB_PKG),yes)
	@echo "✓ Found raylib via pkg-config"
	@echo "  CFLAGS: $(RAYLIB_CFLAGS)"
	@echo "  LDFLAGS: $(RAYLIB_LDFLAGS)"
else ifneq ($(RAYLIB_CFLAGS),)
	@echo "✓ Found raylib at: $(RAYLIB_CFLAGS)"
	@echo "  LDFLAGS: $(RAYLIB_LDFLAGS)"
else
	@echo "✗ raylib not found!"
	@echo ""
	@echo "Please install raylib using one of these methods:"
	@echo "1. Package manager: sudo apt install libraylib-dev (Ubuntu/Debian)"
	@echo "2. From source: make install-raylib"
	@echo "3. Place raylib in one of these directories:"
	@echo "   - /usr/local/include and /usr/local/lib"
	@echo "   - ~/raylib/src"
	@echo "   - ./raylib/src"
	@echo ""
	@echo "If you built raylib from source and see this message, you may need to set PKG_CONFIG_PATH:"
	@echo "  export PKG_CONFIG_PATH=\"/path/to/raylib/pc/files:$$PKG_CONFIG_PATH\""
	@false
endif

# Default goal: build directly when running `make`
.DEFAULT_GOAL := build

# 'all' runs environment checks then builds
all: check-raylib build

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)
	@mkdir -p $(OBJDIR)/2d $(OBJDIR)/3d $(OBJDIR)/data $(OBJDIR)/entity $(OBJDIR)/palette $(OBJDIR)/screen $(OBJDIR)/sprite $(OBJDIR)/util $(OBJDIR)/world

# Build main executable (release)
build: directories $(MAIN_OUT)

$(MAIN_OUT): $(ALL_OBJS)
	$(CC) $(ALL_OBJS) -o $(MAIN_OUT) $(LDFLAGS) -lm

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build main executable (debug)
debug: directories $(DEBUG_OUT)

$(DEBUG_OUT): $(ALL_SRCS)
	$(CC) $(DEBUG_CFLAGS) $(ALL_SRCS) -o $(DEBUG_OUT) $(LDFLAGS) -lm

# Run the game
run: $(MAIN_OUT)
	./$(MAIN_OUT)

# Run debug version
run-debug: $(DEBUG_OUT)
	./$(DEBUG_OUT)

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR) *.missing

# Install raylib from source
install-raylib:
	@echo "Installing raylib from source..."
	@if [ ! -d "$(HOME)/raylib" ]; then \
		echo "Cloning raylib..."; \
		git clone https://github.com/raysan5/raylib.git $(HOME)/raylib; \
	else \
		echo "raylib directory exists, updating..."; \
		cd $(HOME)/raylib && git pull; \
	fi
	@echo "Building raylib..."
	@cd $(HOME)/raylib/src && make PLATFORM=PLATFORM_DESKTOP
	@echo "✓ raylib installed to $(HOME)/raylib"
	@echo "  You can now run 'make' to build the project"

# Environment diagnostics
doctor: check-raylib

# Help target
help:
	@echo "Raylib Starter Pack"
	@echo ""
	@echo "Targets:"
	@echo "  all          - Build the game (with environment checks)"
	@echo "  build        - Build the game (default)"
	@echo "  debug        - Build debug version"
	@echo "  run          - Build and run the game"
	@echo "  run-debug    - Build and run debug version"
	@echo "  clean        - Remove build artifacts"
	@echo "  check-raylib - Check raylib installation"
	@echo "  install-raylib - Install raylib from source"
	@echo "  doctor       - Run environment checks"
	@echo "  windows      - Cross-compile for Windows"
	@echo "  mac          - Build macOS binary"
	@echo "  help         - Show this help message"

.PHONY: all build debug run run-debug clean directories install-raylib check-raylib help doctor

# -----------------------------
# Cross-compile for Windows
# -----------------------------
# Configure cross-compiler
MINGW_CC ?= x86_64-w64-mingw32-gcc
MINGW_CFLAGS ?= -O2 -std=c2x -Wall -Wextra -I/usr/x86_64-w64-mingw32/include -Iinc -Isrc
MINGW_LDFLAGS ?= -L/usr/x86_64-w64-mingw32/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32 -lwinpthread

.PHONY: windows
windows: directories $(WINDOWS_OUT)

$(WINDOWS_OUT): $(ALL_SRCS)
	@if command -v $(MINGW_CC) >/dev/null 2>&1; then \
		echo "Building Windows version..."; \
		if echo '#include "raylib.h"' | $(MINGW_CC) $(MINGW_CFLAGS) -E - >/dev/null 2>&1; then \
			$(MINGW_CC) $(MINGW_CFLAGS) $(ALL_SRCS) -o $(WINDOWS_OUT) $(MINGW_LDFLAGS); \
		else \
			echo "Warning: raylib headers not found for Windows cross-compilation."; \
			echo "Install raylib for MinGW or build raylib for Windows cross-compilation."; \
			touch $(WINDOWS_OUT).missing; \
		fi; \
	else \
		echo "Warning: Cross-compiler '$(MINGW_CC)' not found. Skipping Windows build."; \
		echo "Install mingw-w64 to enable Windows builds: sudo apt install mingw-w64"; \
		touch $(WINDOWS_OUT).missing; \
	fi

# -----------------------------
# Mac OS build target
# -----------------------------
MAC_CC ?= clang
# Add Homebrew include paths for Apple Silicon and Intel Macs
MAC_EXTRA_INCLUDES = -I/opt/homebrew/include -I/usr/local/include
MAC_EXTRA_LIBDIRS = -L/opt/homebrew/lib -L/usr/local/lib

MAC_CFLAGS ?= -O2 -std=c2x -Wall -Wextra $(RAYLIB_CFLAGS) $(MAC_EXTRA_INCLUDES) -Iinc -Isrc
MAC_LDFLAGS ?= $(MAC_EXTRA_LIBDIRS) $(RAYLIB_LDFLAGS) -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -lm

MAC_OUT = $(BINDIR)/game-mac

.PHONY: mac
mac: directories $(MAC_OUT)

$(MAC_OUT): $(ALL_SRCS)
	@if [ "$(shell uname)" != "Darwin" ]; then \
		echo "Note: You're not on macOS; building for macOS may fail unless cross-tools are available."; \
	fi; \
	if command -v $(MAC_CC) >/dev/null 2>&1; then \
		echo "Building macOS binary with $(MAC_CC)..."; \
		$(MAC_CC) $(MAC_CFLAGS) $(ALL_SRCS) -o $(MAC_OUT) $(MAC_LDFLAGS); \
	else \
		echo "Error: '$(MAC_CC)' not found. Install clang or set MAC_CC to an available compiler."; \
		exit 1; \
	fi