# Simple Makefile - this project is simple we dont need a BIG, OP Makefile

# SHELL
RM := rm -rf

# PRETTY PRINTS
define print_cc
	$(if $(Q), @echo "[CC]        $(1)")
endef

define print_bin
	$(if $(Q), @echo "[BIN]       $(1)")
endef

define print_rm
    $(if $(Q), @echo "[RM]        $(1)")
endef

define print
    $(if $(Q), @echo "$(1)")
endef


# ARGS (Quiet OR Verbose), type make V=1 to enable verbose mode
ifeq ("$(origin V)", "command line")
	Q :=
else
	Q ?= @
endif

# DIRS
SDIR := ./src
IDIR := ./include
ADIR := ./app
TDIR := ./test
SCRIPT_DIR := ./scripts

# FILES
SRC := $(wildcard $(SDIR)/*.c)
ASRC := $(SRC) $(wildcard $(ADIR)/*.c)
TSRC := $(SRC) $(wildcard $(TDIR)/*.c)

TEST_LOG_FILE := test_log.txt
POST_EXEC_SCRIPT := $(SCRIPT_DIR)/post_exec_script.sh

LOBJ := $(SRC:%.c=%.o)
AOBJ := $(ASRC:%.c=%.o)
TOBJ := $(TSRC:%.c=%.o)
OBJ := $(AOBJ) $(TOBJ) $(LOBJ)

DEPS := $(OBJ:%.o=%.d)

# LIBS remember -l is added automaticly so type just m for -lm
LIB :=

# BINS
AEXEC := main.out
TEXEC := test.out

# COMPI, DEFAULT GCC
CC ?= gcc

C_STD   := -std=c99
C_OPT   := -O3
C_FLAGS :=
C_WARNS :=

DEP_FLAGS := -MMD -MP
LINKER_FLAGS := -fPIC

H_INC := $(foreach d, $(IDIR), -I$d)
L_INC := $(foreach l, $(LIB), -l$l)

ifeq ($(CC),clang)
	C_WARNS += -Weverything -Wno-padded
else ifneq (, $(filter $(CC), cc gcc))
	C_WARNS += -Wall -Wextra -pedantic -Wcast-align \
			   -Winit-self -Wlogical-op -Wmissing-include-dirs \
			   -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef  \
			   -Wwrite-strings -Wpointer-arith -Wmissing-declarations \
			   -Wuninitialized -Wno-old-style-definition -Wno-old-style-declaration -Wstrict-prototypes \
			   -Wmissing-prototypes -Wswitch-default -Wbad-function-cast \
			   -Wnested-externs -Wconversion -Wunreachable-code
endif

ifeq ("$(origin DEBUG)", "command line")
	GGDB := -ggdb3 -gdwarf-4
	C_OPT := -O0
else
	GGDB :=
endif

C_FLAGS += $(C_STD) $(C_OPT) $(GGDB) $(C_WARNS) $(DEP_FLAGS) $(LINKER_FLAGS)

.PHONY: all
all: app test

.PHONY:app
app: $(AOBJ)
	$(call print_bin,$(AEXEC))
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(AOBJ) -o $(AEXEC) $(L_INC)

.PHONY:test
# Target variable change for test and all childs (subtargets)
test: C_FLAGS += -DTEST_MACRO
test: $(TOBJ)
	$(call print_bin,$(TEXEC))
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(TOBJ) -o $(TEXEC) $(L_INC)

%.o:%.c %.d
	$(call print_cc,$<)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) -c $< -o $@


regression:
	$(call print,Regression has been started:)
	$(call print,cleaning:)
	$(Q)$(MAKE) clean --no-print-directory
	$(call print,building app:)
	$(Q)$(MAKE) app --no-print-directory
	$(call print,cleaning:)
	$(Q)$(MAKE) clean --no-print-directory
	$(call print,building tests:)
	$(Q)$(MAKE) test --no-print-directory
	$(call print,running tests:)
	$(Q)./$(TEXEC) > $(TEST_LOG_FILE)
	$(Q)$(POST_EXEC_SCRIPT) $(TEST_LOG_FILE)
	$(call print,Regression PASSED)

.PHONY: memcheck
memcheck: test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./test.out

.PHONY: clean
clean:
	$(call print_rm,EXEC)
	$(Q)$(RM) $(AEXEC) $(TEXEC)
	$(call print_rm,OBJ)
	$(Q)$(RM) $(OBJ)
	$(Q)$(RM) $(TEST_LOG_FILE)
	$(call print_rm,DEPS)
	$(Q)$(RM) $(DEPS)

.PHONY: help
help:
	@echo "Makefile"
	@echo -e
	@echo "Targets:"
	@echo "    all               - build app and test"
	@echo "    app               - build only app"
	@echo "    test              - build only test"
	@echo "    regression        - build and run all tests"\
	@echo "    memcheck          - build test and run them using valgrind"
	@echo -e
	@echo "Makefile supports Verbose mode when V=1 (make all V=1)"
	@echo "Makefile supports Debug mode when DEBUG=1 (make all DEBUG=1)"
	@echo "To change default compiler (gcc) change CC variable (i.e export CC=clang)"

# TRICK TO AUTO MANAGE DEPS
$(DEPS):


include $(wildcard $(DEPS))