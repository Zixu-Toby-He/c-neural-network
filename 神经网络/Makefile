# 跨平台命令
ifeq ($(OS),Windows_NT)

SHELL            := powershell.exe
.SHELLFLAGS      := -NoProfile -ExecutionPolicy Bypass -Command
EXEEXT           := .exe
NEURAL_LIB_NAME  := neural.dll
PLATFORM_NAME    := windows
DEFAULT_CC       := gcc
UTF8_PREFIX      = chcp 65001 > $$null; $$OutputEncoding = [System.Text.UTF8Encoding]::new(); [Console]::OutputEncoding = $$OutputEncoding;
CHECK_COMPILER   =
UTF8_CHARSET_FLAGS = -finput-charset=UTF-8 -fexec-charset=UTF-8
DIAGNOSTICS_CHARSET_FLAG =
MKDIR            = $(UTF8_PREFIX) New-Item -ItemType Directory -Force $(subst /,\,$(1)) | Out-Null
COPY_DIR         = $(UTF8_PREFIX) if (Test-Path "$(subst /,\,$(1))") { Copy-Item -Recurse -Force -ErrorAction SilentlyContinue "$(subst /,\,$(1))\*" "$(subst /,\,$(2))" }
RM_RF            = $(UTF8_PREFIX) if (Test-Path $(subst /,\,$(1))) { Remove-Item -Recurse -Force $(subst /,\,$(1)) }

else

SHELL           := bash
EXEEXT          :=
NEURAL_LIB_NAME := libneural.so
PLATFORM_NAME   := linux
LOCALE          ?= C.UTF-8
DEFAULT_CC       := $(or $(firstword $(foreach candidate,gcc-14 gcc-13 gcc-12 gcc-11 gcc-10 gcc,$(shell command -v $(candidate) 2>/dev/null))),gcc)
UTF8_PREFIX      = env LANG=$(LOCALE) LC_ALL=$(LOCALE)
CHECK_COMPILER   = printf 'int 中文标识符 = 0; int main(void){return 中文标识符;}\n' | $(CC) $(STD_FLAGS) $(UTF8_CHARSET_FLAGS) -x c - -c -o /dev/null >/dev/null 2>&1 || { echo "$(CC) does not support UTF-8 Chinese identifiers in C. Please install GCC 10+ or run make CC=/path/to/newer-gcc."; exit 1; }
UTF8_CHARSET_FLAGS = -finput-charset=UTF-8 -fexec-charset=UTF-8
export LANG      := $(LOCALE)
export LC_ALL    := $(LOCALE)
MKDIR            = mkdir -p $(1)
COPY_DIR         = if [ -d "$(1)" ]; then cp -R -f "$(1)/." "$(2)/"; fi
RM_RF            = rm -rf $(1)

endif

# 编译器与编译选项
ifeq ($(origin CC),default)
CC := $(DEFAULT_CC)
endif

OPT_FLAGS        = -O2
STD_FLAGS        = -std=gnu11 -fextended-identifiers

ifeq ($(OS),Windows_NT)
ifeq ($(ENABLE_DIAGNOSTICS_CHARSET),1)
DIAGNOSTICS_CHARSET_FLAG = -fdiagnostics-charset=UTF-8
endif
else
cc-option = $(shell printf 'int main(void){return 0;}\n' | $(CC) $(1) -x c - -c -o /dev/null >/dev/null 2>&1 && printf '%s' '$(1)')
DIAGNOSTICS_CHARSET_FLAG := $(call cc-option,-fdiagnostics-charset=UTF-8)
endif

CHARSET_FLAGS    = $(UTF8_CHARSET_FLAGS) $(DIAGNOSTICS_CHARSET_FLAG)
BASE_CFLAGS ?= -Wall -Wextra \
	$(STD_FLAGS) $(CHARSET_FLAGS) $(OPT_FLAGS)
LDFLAGS ?=
LDLIBS  ?= -lm

BUILD_ROOT       := build/$(PLATFORM_NAME)
NEURAL_BUILD_DIR := $(BUILD_ROOT)/neural
MAIN_BUILD_ROOT  := $(BUILD_ROOT)/main
RUN_ROOT_DIR     := run/$(PLATFORM_NAME)

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

.PHONY: all neural check-compiler prepare_neural clean run env-info $(MAIN_NAMES) $(addprefix run-,$(MAIN_NAMES))
.SILENT:

all: neural $(MAIN_NAMES)

neural: $(NEURAL_TARGET)

check-compiler:
	$(UTF8_PREFIX) $(CHECK_COMPILER)

prepare_neural: check-compiler
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

env-info:
	@echo "OS=$(OS)"
	@echo "PLATFORM_NAME=$(PLATFORM_NAME)"
	@echo "SHELL=$(SHELL)"
	@echo "CC=$(CC)"
	@echo "LOCALE=$(LOCALE)"
	@echo "BASE_CFLAGS=$(BASE_CFLAGS)"
	@echo "NEURAL_BUILD_DIR=$(NEURAL_BUILD_DIR)"
	@echo "RUN_ROOT_DIR=$(RUN_ROOT_DIR)"
	@$(UTF8_PREFIX) locale
	@$(UTF8_PREFIX) $(CC) --version | head -n 1

clean:
	$(call RM_RF,build)
	$(call RM_RF,run)
