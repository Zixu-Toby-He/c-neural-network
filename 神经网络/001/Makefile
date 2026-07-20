# 跨平台命令
ifeq ($(OS),Windows_NT)

SHELL        := powershell.exe
.SHELLFLAGS  := -NoProfile -ExecutionPolicy Bypass -Command
EXEEXT       := .exe
UTF8_PREFIX  = chcp 65001 > $$null; $$OutputEncoding = [System.Text.UTF8Encoding]::new(); [Console]::OutputEncoding = $$OutputEncoding;
CHARSET_FLAGS = -finput-charset=UTF-8 -fexec-charset=UTF-8
MKDIR         = $(UTF8_PREFIX) New-Item -ItemType Directory -Force $(subst /,\,$(1)) | Out-Null
COPY_FILE     = $(UTF8_PREFIX) Copy-Item -Force $(subst /,\,$(1)) $(subst /,\,$(2))
COPY_RESOURCE = $(UTF8_PREFIX) Copy-Item -Force build/resource/* run
RM_RF         = $(UTF8_PREFIX) if (Test-Path $(subst /,\,$(1))) { Remove-Item -Recurse -Force $(subst /,\,$(1)) }

else

EXEEXT       :=
UTF8_PREFIX  =
CHARSET_FLAGS = -finput-charset=UTF-8 -fexec-charset=UTF-8
MKDIR         = mkdir -p $(1)
COPY_FILE     = cp -f $(1) $(2)
COPY_RESOURCE = cp -f build/resource/* run/
RM_RF         = rm -rf $(1)

endif

# 编译器与编译选项
CC = gcc
CFLAGS ?= -Wall -Wextra \
	$(CHARSET_FLAGS) \
	-I$(MAIN_SRC_DIR) \
	-Isc/neural/include \
	-Isc/dataset/include
LDFLAGS ?=
LDLIBS ?= -lm

MAIN_BUILD_DIR     := build/main
NEURAL_BUILD_DIR   := build/neural
DATASET_BUILD_DIR  := build/dataset
RESOURCE_BUILD_DIR := build/resource
RUN_DIR            := run

MAIN_NAME ?= 1551_rre_train_x_times_x
MAIN_SRC_DIR := sc/main/$(MAIN_NAME)
TARGET := $(MAIN_BUILD_DIR)/$(MAIN_NAME)$(EXEEXT)
RUN_TARGET := $(RUN_DIR)/$(MAIN_NAME)$(EXEEXT)

MAIN_SRCS    := $(wildcard $(MAIN_SRC_DIR)/*.c)
NEURAL_SRCS  := $(wildcard sc/neural/src/*.c)
DATASET_SRCS := $(wildcard sc/dataset/src/*.c)

MAIN_OBJS    := $(patsubst $(MAIN_SRC_DIR)/%.c,$(MAIN_BUILD_DIR)/%.o,$(MAIN_SRCS))
NEURAL_OBJS  := $(patsubst sc/neural/src/%.c,$(NEURAL_BUILD_DIR)/%.o,$(NEURAL_SRCS))
DATASET_OBJS := $(patsubst sc/dataset/src/%.c,$(DATASET_BUILD_DIR)/%.o,$(DATASET_SRCS))
OBJS         := $(MAIN_OBJS) $(NEURAL_OBJS) $(DATASET_OBJS)

.PHONY: all prepare run clean
.SILENT:

all: $(RUN_TARGET)

prepare:
	$(call MKDIR,$(MAIN_BUILD_DIR))
	$(call MKDIR,$(NEURAL_BUILD_DIR))
	$(call MKDIR,$(DATASET_BUILD_DIR))
	$(call MKDIR,$(RESOURCE_BUILD_DIR))
	$(call MKDIR,$(RUN_DIR))

$(MAIN_BUILD_DIR)/%.o: $(MAIN_SRC_DIR)/%.c | prepare
	$(UTF8_PREFIX) $(CC) $(CFLAGS) -c $< -o $@

$(NEURAL_BUILD_DIR)/%.o: sc/neural/src/%.c | prepare
	$(UTF8_PREFIX) $(CC) $(CFLAGS) -c $< -o $@

$(DATASET_BUILD_DIR)/%.o: sc/dataset/src/%.c | prepare
	$(UTF8_PREFIX) $(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | prepare
	$(UTF8_PREFIX) $(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

$(RESOURCE_BUILD_DIR)/randnum.bin: resource/randnum.bin | prepare
	$(call COPY_FILE,$<,$@)

$(RESOURCE_BUILD_DIR)/py_visualize.py: resource/py_visualize.py | prepare
	$(call COPY_FILE,$<,$@)

$(RUN_TARGET): $(TARGET) $(RESOURCE_BUILD_DIR)/randnum.bin $(RESOURCE_BUILD_DIR)/py_visualize.py | prepare
	$(call COPY_FILE,$(TARGET),$@)
	$(COPY_RESOURCE)

run: all
	$(UTF8_PREFIX) ./$(RUN_TARGET)

clean:
	$(call RM_RF,build)
	$(call RM_RF,run)
