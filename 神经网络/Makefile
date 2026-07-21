# 跨平台命令
ifeq ($(OS),Windows_NT)

SHELL           := powershell.exe
.SHELLFLAGS     := -NoProfile -ExecutionPolicy Bypass -Command
EXEEXT          := .exe
NEURAL_LIB_NAME := neural.dll
UTF8_PREFIX      = chcp 65001 > $$null; $$OutputEncoding = [System.Text.UTF8Encoding]::new(); [Console]::OutputEncoding = $$OutputEncoding;
CHARSET_FLAGS    = -finput-charset=UTF-8 -fexec-charset=UTF-8
MKDIR            = $(UTF8_PREFIX) New-Item -ItemType Directory -Force $(subst /,\,$(1)) | Out-Null
COPY_DIR         = $(UTF8_PREFIX) if (Test-Path "$(subst /,\,$(1))") { Copy-Item -Recurse -Force -ErrorAction SilentlyContinue "$(subst /,\,$(1))\*" "$(subst /,\,$(2))" }
RM_RF            = $(UTF8_PREFIX) if (Test-Path $(subst /,\,$(1))) { Remove-Item -Recurse -Force $(subst /,\,$(1)) }

else

SHELL           := bash
EXEEXT          :=
NEURAL_LIB_NAME := libneural.so
UTF8_PREFIX      =
CHARSET_FLAGS    = -finput-charset=UTF-8 -fexec-charset=UTF-8
MKDIR            = mkdir -p $(1)
COPY_DIR         = if [ -d "$(1)" ]; then cp -R -f "$(1)/." "$(2)/"; fi
RM_RF            = rm -rf $(1)

endif

# 编译器与编译选项
CC = gcc
OPT_FLAGS        = -O2
BASE_CFLAGS ?= -Wall -Wextra \
	$(CHARSET_FLAGS) $(OPT_FLAGS)
LDFLAGS ?=
LDLIBS  ?= -lm

NEURAL_BUILD_DIR := build/neural
MAIN_BUILD_ROOT  := build/main
RUN_ROOT_DIR     := run

MAIN_NAMES := $(notdir $(patsubst %/,%,$(wildcard sc/main/*/)))

NEURAL_SRCS   := $(wildcard sc/neural/src/*.c)
NEURAL_OBJS   := $(patsubst sc/neural/src/%.c,$(NEURAL_BUILD_DIR)/%.o,$(NEURAL_SRCS))
NEURAL_TARGET := $(RUN_ROOT_DIR)/$(NEURAL_LIB_NAME)

ifeq ($(OS),Windows_NT)

NEURAL_CFLAGS     := $(BASE_CFLAGS) -Isc/neural/include
MAIN_LINK_FLAGS   :=
NEURAL_LINK_FLAGS := -shared '-Wl,--out-implib,$(NEURAL_BUILD_DIR)/libneural.dll.a'
MAIN_NEURAL_LIB   := -L$(NEURAL_BUILD_DIR) -lneural

else

NEURAL_CFLAGS     := $(BASE_CFLAGS) -Isc/neural/include -fPIC
MAIN_LINK_FLAGS   := -Wl,-rpath,'$$ORIGIN'
NEURAL_LINK_FLAGS := -shared
MAIN_NEURAL_LIB   := -L$(RUN_ROOT_DIR) -lneural

endif

.PHONY: all neural prepare_neural clean run $(MAIN_NAMES) $(addprefix run-,$(MAIN_NAMES))
.SILENT:

all: neural $(MAIN_NAMES)

neural: $(NEURAL_TARGET)

prepare_neural:
	$(call MKDIR,$(NEURAL_BUILD_DIR))
	$(call MKDIR,$(RUN_ROOT_DIR))

$(NEURAL_BUILD_DIR)/%.o: sc/neural/src/%.c | prepare_neural
	$(UTF8_PREFIX) $(CC) $(NEURAL_CFLAGS) -c $< -o $@

$(NEURAL_TARGET): $(NEURAL_OBJS) | prepare_neural
	$(UTF8_PREFIX) $(CC) $(NEURAL_LINK_FLAGS) $(NEURAL_OBJS) $(LDFLAGS) $(LDLIBS) -o $@

define DEFINE_MAIN_PROJECT
MAIN_SRCS_$1 := $$(wildcard sc/main/$1/*.c)
MAIN_OBJS_$1 := $$(patsubst sc/main/$1/%.c,$(MAIN_BUILD_ROOT)/$1/%.o,$$(MAIN_SRCS_$1))
MAIN_CFLAGS_$1 := $(BASE_CFLAGS) -Isc/main/$1 -Isc/neural/include

.PHONY: prepare_main_$1
prepare_main_$1: prepare_neural
	$$(call MKDIR,$(MAIN_BUILD_ROOT)/$1)

$(MAIN_BUILD_ROOT)/$1/%.o: sc/main/$1/%.c | prepare_main_$1
	$$(UTF8_PREFIX) $$(CC) $$(MAIN_CFLAGS_$1) -c $$< -o $$@

$(RUN_ROOT_DIR)/$1$(EXEEXT): $$(MAIN_OBJS_$1) $(NEURAL_TARGET) | prepare_main_$1
	$$(UTF8_PREFIX) $$(CC) $$(MAIN_OBJS_$1) $$(MAIN_LINK_FLAGS) $$(MAIN_NEURAL_LIB) $$(LDFLAGS) $$(LDLIBS) -o $$@

$1: $(RUN_ROOT_DIR)/$1$(EXEEXT)
	$$(call COPY_DIR,resource/$1,$(RUN_ROOT_DIR))

run-$1: $1
	$$(UTF8_PREFIX) ./$(RUN_ROOT_DIR)/$1$(EXEEXT)
endef

$(foreach MAIN,$(MAIN_NAMES),$(eval $(call DEFINE_MAIN_PROJECT,$(MAIN))))

run: run-$(firstword $(MAIN_NAMES))

clean:
	$(call RM_RF,build)
	$(call RM_RF,run)
