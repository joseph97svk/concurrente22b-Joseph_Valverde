# C,C++,Java,Py Makefile v2.8.2 2022-Nov-30 Jeisson Hidalgo ECCI-UCR CC-BY 4.0

# Compiler and tool flags
CC=gcc
XC=g++
JC=javac
DEFS=
CSTD=-std=gnu11
XSTD=-std=gnu++11
FLAG=
FLAGS=$(strip -Wall -Wextra $(FLAG) $(DEFS))
FLAGC=$(FLAGS) $(CSTD)
FLAGX=$(FLAGS) $(XSTD)
LIBS=
LINTF=-build/header_guard,-build/include_subdir
LINTC=$(LINTF),-readability/casting
LINTX=$(LINTF),-build/c++11,-runtime/references
ARGS=

# Directories
RSC_DIR=assets
BIN_DIR=bin
OBJ_DIR=build
DOC_DIR=doc
SRC_DIR=src
TST_DIR=tests

# If src/ dir does not exist, use current directory .
ifeq "$(wildcard $(SRC_DIR) )" ""
	SRC_DIR=.
endif

# Files
DIRS=$(shell find -L $(SRC_DIR) -type d)
ASSETS=$(wildcard $(RSC_DIR)/)
APPNAME=$(shell basename $(shell pwd))
HEADERC=$(wildcard $(DIRS:%=%/*.h))
HEADERX=$(wildcard $(DIRS:%=%/*.hpp))
SOURCEC=$(wildcard $(DIRS:%=%/*.c))
SOURCEX=$(wildcard $(DIRS:%=%/*.cpp))
SOURCEJ=$(wildcard $(DIRS:%=%/*.java))
SOURCEP=$(wildcard $(DIRS:%=%/*.py))
INPUTFC=$(strip $(HEADERC) $(SOURCEC))
INPUTFX=$(strip $(HEADERX) $(SOURCEX))
INPUTCX=$(strip $(INPUTFC) $(INPUTFX))
OBJECTC=$(SOURCEC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJECTX=$(SOURCEX:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJECTJ=$(SOURCEJ:$(SRC_DIR)/%.java=$(OBJ_DIR)/%.class)
OBJECTS=$(strip $(OBJECTC) $(OBJECTX))
TESTINP=$(wildcard $(TST_DIR)/input*.txt)
INCLUDE=$(DIRS:%=-I%)
DEPENDS=$(OBJECTS:%.o=%.d)
REMOVES=$(BIN_DIR)/ $(OBJ_DIR)/ $(DOC_DIR)/
IGNORES=$(REMOVES) *.pyc .vscode
EXEFILE=$(BIN_DIR)/$(APPNAME)
MAINCLS=$(shell grep -lE 'static\s+void\s+main\b' $(SRC_DIR)/*.java | xargs basename -s .java)
LD=$(if $(SOURCEX),$(XC),$(CC))
ifneq ($(OBJECTS),)
	TARGETS+=$(EXEFILE)
	TESTOUT+=$(TESTINP:$(TST_DIR)/input%.txt=exe/output%.txt)
	OUTPUTX+=$(TESTINP:$(TST_DIR)/input%.txt=cppout/output%.txt)
	EXEARGS+=$(strip $(EXEFILE) $(ARGS))
	DOCTARG+=cppdoc
endif
ifneq ($(OBJECTJ),)
	TARGETS+=$(EXEFILE).jar
	TESTOUT+=$(TESTINP:$(TST_DIR)/input%.txt=jar/output%.txt)
	OUTPUTJ+=$(TESTINP:$(TST_DIR)/input%.txt=javaout/output%.txt)
	JARARGS=$(strip java -jar $(EXEFILE).jar $(ARGS))
	DOCTARG+=javadoc
endif
ifneq ($(SOURCEP),)
	TESTOUT+=$(TESTINP:$(TST_DIR)/input%.txt=py3/output%.txt)
	OUTPUTP+=$(TESTINP:$(TST_DIR)/input%.txt=pyout/output%.txt)
	PY3ARGS=$(strip python3 $(SOURCEP) $(ARGS))
	DOCTARG+=pydoc
endif

# Targets
default: debug
all: debug test lint doc
debug: FLAG += -g
debug: $(TARGETS)
release: FLAGS += -O3 -DNDEBUG
release: $(TARGETS)
asan: FLAGS += -fsanitize=address -fno-omit-frame-pointer
asan: debug
msan: FLAGS += -fsanitize=memory
msan: CC = clang
msan: XC = clang++
msan: debug
tsan: FLAGS += -fsanitize=thread
tsan: debug
ubsan: FLAGS += -fsanitize=undefined
ubsan: debug

-include *.mk $(DEPENDS)
.SECONDEXPANSION:

# C/C++ Linker call
$(EXEFILE): $(OBJECTS) | $$(@D)/.
	$(LD) $(FLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

# Compile C source file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $$(@D)/.
	$(CC) -c $(FLAGC) $(INCLUDE) -MMD $< -o $@

# Compile C++ source file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $$(@D)/.
	$(XC) -c $(FLAGX) $(INCLUDE) -MMD $< -o $@

# Java Linker call. TODO(jhc): fallible main class detection
%.jar: $(OBJECTJ) | $$(@D)/.	
	jar cfe $@ $(MAINCLS) -C $(OBJ_DIR) .
ifneq ($(ASSETS),)
	jar uf $@ -C $(ASSETS) .
endif

# Compile Java source file
.PRECIOUS: $(OBJ_DIR)/%.class
$(OBJ_DIR)/%.class: $(SRC_DIR)/%.java | $$(@D)/.
	$(strip $(JC) -classpath $(SRC_DIR) $(FLAG)) $< -d $(OBJ_DIR)

# Create a subdirectory if not exists
.PRECIOUS: %/.
%/.:
	mkdir -p $(dir $@)

# Test cases
.PHONY: test doc lint run memcheck helgrind gitignore clean instdeps
test: SHELL:=/bin/bash
test: $(TARGETS) $(TESTOUT)

# TODO(any): Remove redundancy
exe/output%.txt: $(TST_DIR)/input%.txt $(TST_DIR)/output%.txt
	icdiff --no-headers $(word 2,$^) <($(EXEARGS) < $<)

jar/output%.txt: $(TST_DIR)/input%.txt $(TST_DIR)/output%.txt
	icdiff --no-headers $(word 2,$^) <($(JARARGS) < $<)

py3/output%.txt: $(TST_DIR)/input%.txt $(TST_DIR)/output%.txt
	icdiff --no-headers $(word 2,$^) <($(PY3ARGS) < $<)

# Test case output files
cppout: $(TARGETS) $(OUTPUTX)
cppout/output%.txt: $(TST_DIR)/input%.txt $(TST_DIR)/output%.txt
	$(JARARGS) < $< > $(word 2,$^)

javaout: $(TARGETS) $(OUTPUTJ)
javaout/output%.txt: $(TST_DIR)/input%.txt $(TST_DIR)/output%.txt
	$(JARARGS) < $< > $(word 2,$^)

pyout: $(TARGETS) $(OUTPUTP)
pyout/output%.txt: $(TST_DIR)/input%.txt $(TST_DIR)/output%.txt
	$(JARARGS) < $< > $(word 2,$^)

# Documentation
doc: $(DOCTARG)
cppdoc: Doxyfile $(INPUTCX)
	doxygen -q

javadoc: $(SOURCEJ) | $(DOC_DIR)/java/.
	javadoc -quiet $(SOURCEJ) -d $(DOC_DIR)/java

pydoc: $(SOURCEP) | $(DOC_DIR)/python/.
	pydoc -w $(SOURCEP)
	mv *.html $(DOC_DIR)/python

Doxyfile:
	-doxygen -g -s
	sed -i.bak -E 's/(OUTPUT_DIRECTORY *)=/\1= $(DOC_DIR)/' $@
	sed -i.bak -E 's/(INPUT *)=/\1= $(SRC_DIR)/' $@
	sed -i.bak -E 's/(RECURSIVE *)= NO/\1= YES/' $@
	sed -i.bak -E 's/(EXTRACT_ALL *)= NO/\1= YES/' $@
	rm -f $@.bak

# Code style
lint:
ifneq ($(INPUTFC),)
	-cpplint --filter=$(LINTC) $(INPUTFC)
endif
ifneq ($(INPUTFX),)
	-cpplint --filter=$(LINTX) $(INPUTFX)
endif
ifneq ($(SOURCEJ),)
	-checkstyle -c /google_checks.xml $(SOURCEJ)
	-checkstyle -c /sun_checks.xml $(SOURCEJ)
endif
ifneq ($(SOURCEP),)
	-pylint -sn $(SOURCEP)
endif

run: $(EXEFILE)
	$(EXEARGS)

memcheck: $(EXEFILE)
	valgrind --tool=memcheck $(EXEARGS)

helgrind: $(EXEFILE)
	valgrind --quiet --tool=helgrind $(EXEARGS)

gitignore:
	echo $(IGNORES) | tr " " "\n" > .gitignore

clean:
	rm -rf $(REMOVES)
ifneq ($(SOURCEP),)
	find $(SRC_DIR) -name '*.pyc' -exec rm -f {} \;
endif

# Install dependencies (Debian)
instdeps:
	sudo apt install build-essential clang valgrind icdiff doxygen graphviz \
	python3-pip python3-gpg openjdk-17-jdk checkstyle && sudo pip3 install cpplint

help:
	@echo "Usage make [-jN] [VAR=value] [target]"
	@echo "  -jN       Compile N files simultaneously [N=1]"
	@echo "  VAR=value Overrides a variable, e.g CC=mpicc DEFS=-DGUI. See below"
	@echo "  all       Run targets: test lint doc"
	@echo "  asan      Build for detecting memory leaks and invalid accesses"
	@echo "  clean     Remove generated directories and files"
	@echo "  debug     Build an executable for debugging [default]"
	@echo "  doc       Generate documentation from sources"
	@echo "  gitignore Generate a .gitignore file"
	@echo "  helgrind  Run executable for detecting thread errors with Valgrind"
	@echo "  instdeps  Install needed packages on Debian-based distributions"
	@echo "  lint      Check code style conformance using Cpplint"
	@echo "  memcheck  Run executable for detecting memory errors with Valgrind"
	@echo "  msan      Build for detecting uninitialized memory usage"
	@echo "  <L>out    Generate test case output using language L: cpp|java|py"
	@echo "  release   Build an optimized executable"
	@echo "  run       Run executable using ARGS value as arguments"
	@echo "  test      Run executable against test cases in folder tests/"
	@echo "  tsan      Build for detecting thread errors, e.g race conditions"
	@echo "  ubsan     Build for detecting undefined behavior"
	@echo "Variables:"
	@echo "  CC: C compiler. XC: C++ compiler. C/XSTD: Force a C/C++ standard"
	@echo "  DEFS: preprocessor symbols. FLAG: compiler flags"
	@echo "  LIBS: libraries to be linked"
	@echo "  ARGS: execution arguments"
