
# Clear implicit rules to prevent Make from using the wrong compiler and commands
.SUFFIXES :

# Project name
PROJECT := fragtest

CC := clang

# MacOS linker doesn't support wrap :( ... could use _interpose maybe later
WRAP_FLAGS := \
	-Dwrite=__wrap_write

ASAN_FLAGS := \
	-fsanitize=address \
	-fno-omit-frame-pointer \
	-fsanitize-address-use-after-scope \
	-fno-optimize-sibling-calls \
	-fno-common

LDFLAGS := \
	$(ASAN_FLAGS)

C_FLAGS := -std=c23 -g3 -O0 \
	-D_POSIX_C_SOURCE=200809L \
	$(WRAP_FLAGS) \
	$(ASAN_FLAGS)

PRE_FLAGS := -MMD -MP

# Source and includes
SRC := \
	src \
	umm

INC := \
	inc

LIB := lib

# Build directories and output
BUILD := build/$(PROJECT)
TARGET := $(BUILD)/$(PROJECT)

# Library search directories and flags
EXT_LIB :=
LDPATHS := # $(addprefix -L,$(LIB) $(EXT_LIB)) disabled, no libraries

# Include directories
INC_DIRS := $(INC)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Custom mapping of source to object file to handle the common code in the parent directory
SRC_TO_OBJ_MAPPER = $(addprefix $(BUILD)/,$(subst ../,,$(addsuffix .o,$(basename $(1)))))

# Construct build output and dependency filenames
SRCS := $(shell find $(SRC) -name *.c)
OBJS := $(call SRC_TO_OBJ_MAPPER,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Main task
all: $(TARGET)

# Build task
build: all

# Task producing target from object files
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDPATHS) $(LDFLAGS)
	@echo

# Template to create rules using two parameters: object and source
define OBJ_RULE
$(1): $(2)
	@mkdir -p $(dir $1)
	$(CC) $(C_FLAGS) $(PRE_FLAGS) $(INC_FLAGS) -c -o $1 $2 $(LDPATHS) $(LDFLAGS)
endef

# Generate object build rules using the template, necessary because of the custom mapping
$(foreach _src,$(SRCS),$(eval $(call OBJ_RULE,$(call SRC_TO_OBJ_MAPPER, $(_src)),$(_src))))

# Clean task
.PHONY: clean
clean:
	@echo "Cleaning..."
	rm -rf $(BUILD)

# Include all dependencies
-include $(DEPS)
