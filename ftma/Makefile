##
##  Template makefile for Standard, Profile, Debug, Release, and Release-static versions
##
##    eg: "make rs" for a statically linked release version.
##        "make d"  for a debug version (no optimizations).
##        "make"    for the standard version (optimized, but with debug information and assertions active)

PWD        = $(shell pwd)
EXEC       = gmodel

WROOT      = $(PWD)/src
LIB        = gmodel

MAINFILE   = main

SRCSUF     = cpp

CSRCS      = $(wildcard $(WROOT)/*.$(SRCSUF)) 
DSRCS      = $(foreach dir, $(DEPDIR), $(filter-out $(WROOT)/$(dir)/$(MAINFILE).$(SRCSUF), $(wildcard $(WROOT)/$(dir)/*.$(SRCSUF))))
CHDRS      = $(wildcard $(WROOT)/*.h)
COBJS      = $(CSRCS:.$(SRCSUF)=.o) $(DSRCS:.$(SRCSUF)=.o)

ACOBJS      = $(CSRCS:.$(SRCSUF)=.ast) $(DSRCS:.$(SRCSUF)=.ast)

PCOBJS     = $(addsuffix p,  $(COBJS))
DCOBJS     = $(addsuffix d,  $(COBJS))
CCOBJS     = $(addsuffix c,  $(COBJS))
RCOBJS     = $(addsuffix r,  $(COBJS))

ifeq ($(SRCSUF),cpp)

	CC=g++  -std=c++11  -Wno-deprecated-register

	CLANG=clang++
    CFLAGS     =  -I include -I ../../raptor/include -pedantic -W -Werror      -DHAVE_CONFIG_H -Wlong-long -Wno-unused-parameter -Wno-delete-non-virtual-dtor #-std=c++11 
#LFLAGS    =   -pedantic -W -Werror
else
	CC=gcc
	CLANG=clang

	CFLAGS     =  -pedantic -W -Wall      -DHAVE_CONFIG_H -Wlong-long

	LFLAGS    =   -pedantic -W -Wall 
endif


#CFLAGS+= -fdiagnostics-absolute-paths #the error path is absolute path
# COPTIMIZE	= -m64 -Ofast -flto -march=native  -funroll-loops
COPTIMIZE ?= -O2 


#LFLAGS    +=-lglog



.PHONY : all s p d r c rs clean  test ast




s:	$(EXEC)
p:	$(EXEC)_profile
d:	$(EXEC)_debug  
r:	$(EXEC)_release
c:	$(EXEC)_check
rs:	$(EXEC)_static
ast:	$(EXEC)_ast

libs:	lib$(LIB)_standard.a
libp:	lib$(LIB)_profile.a
libd:	lib$(LIB)_debug.a
libc:	lib$(LIB)_check.a
libr:	lib$(LIB)_release.a

## Compile options
%.o:			CFLAGS +=$(COPTIMIZE) -g -D DEBUG
%.op:			CFLAGS +=$(COPTIMIZE) -pg -g -D NDEBUG
%.od:			CFLAGS +=-O0 -g -D DEBUG
%.oc:			
%.or:			CFLAGS +=$(COPTIMIZE)  -D NDEBUG

## Link options
$(EXEC):		LFLAGS += -g
$(EXEC)_profile:	LFLAGS += -g -pg
$(EXEC)_debug:		LFLAGS += -g
#$(EXEC)_check:	LFLAGS += 
#$(EXEC)_release:	LFLAGS += 
$(EXEC)_static:		LFLAGS += --static
$(EXEC)_ast:		CFLAGS += -emit-ast

## Dependencies
$(EXEC):		$(COBJS)
$(EXEC)_profile:	$(PCOBJS)
$(EXEC)_debug:		$(DCOBJS) 
$(EXEC)_check:		$(CCOBJS)
$(EXEC)_release:	$(RCOBJS)
$(EXEC)_static:		$(RCOBJS)
$(EXEC)_ast:		$(ACOBJS)

lib$(LIB)_standard.a:	$(filter-out */$(MAINFILE).o,  $(COBJS))
lib$(LIB)_profile.a:	$(filter-out */$(MAINFILE).op, $(PCOBJS))
lib$(LIB)_debug.a:	$(filter-out */$(MAINFILE).od, $(DCOBJS))
lib$(LIB)_check.a:	$(filter-out */$(MAINFILE).oc, $(CCOBJS))
lib$(LIB)_release.a:	$(filter-out */$(MAINFILE).or, $(RCOBJS))


parser: LEX YACC

LEX: src/uppaalscan.l
	lex  -o include/io/uppaalscan.h src/uppaalscan.l

YACC: src/uppaalparser.y
	bison  -o  src/uppaalparser.cpp  -v src/uppaalparser.y

## Build rule
%.o %.op %.od %.or :%.$(SRCSUF)   
	@echo Compiling: $(subst $(WROOT)/,,$@)
	@$(CC) $(CFLAGS) -c -o $@ $<

%.ast: %.$(SRCSUF)
	@echo Compiling: $(subst $(WROOT)/,,$@)
	@$(CC) $(CFLAGS) $< -o $@ 


## Build rule
%.oc:	%.$(SRCSUF)
	@echo Compiling: $(subst $(WROOT)/,,$@)
	@$(CLANG) $(CFLAGS) -c -o $@ $<


## Linking rules (standard/profile/debug/release)
$(EXEC)  $(EXEC)_profile $(EXEC)_debug $(EXEC)_release $(EXEC)_static :
	@echo Linking: "$@ ( $(foreach f,$^,$(subst $(WROOT)/,,$f)) )"
	@$(CC) $^  -o $@ $(LFLAGS)

## Linking rules (standard/profile/debug/release)
$(EXEC)_check:
	@echo Linking: "$@ ( $(foreach f,$^,$(subst $(WROOT)/,,$f)) )"
	@$(CLANG) $^  -o $@ $(LFLAGS)


## Library rules (standard/profile/debug/release)
lib$(LIB)_standard.a lib$(LIB)_profile.a lib$(LIB)_release.a lib$(LIB)_debug.a lib$(LIB)_check.a:
	@echo Making library: "$@ ( $(foreach f,$^,$(subst $(WROOT)/,,$f)) )"
	@$(AR) -rcsv $@ $^

## Library Soft Link rule:
libs libp libd libr:
	@echo "Making Soft Link: $^ -> lib$(LIB).a"
	@ln -sf $(PWD)/$^ lib$(LIB).a

## Clean rule
clean: 
	@rm -f $(EXEC) $(EXEC)_profile $(EXEC)_debug $(EXEC)_release $(EXEC)_static \
	  $(COBJS) $(PCOBJS) $(DCOBJS) $(RCOBJS) $(CCOBJS)  $(ACOBJS) *.core depend.mk \
	   lib$(LIB)_standard.a lib$(LIB)_profile.a lib$(LIB)_release.a lib$(LIB)_debug.a


test:
	test/runtest.sh  test

## Make dependencies
depend.mk: $(CSRCS) $(CHDRS)
	@echo Making dependencies
	@$(CC) $(CFLAGS) -I$(WROOT) \
	   $(CSRCS) -MM | sed 's|\(.*\):|$(WROOT)/\1 $(WROOT)/\1r $(WROOT)/\1d $(WROOT)/\1p:|' > depend.mk
	@for dir in $(DEPDIR); do \
	      if [ -r $(WROOT)/$${dir}/depend.mk ]; then \
		  echo Depends on: $${dir}; \
		  cat $(WROOT)/$${dir}/depend.mk >> depend.mk; \
	      fi; \
	  done

#-include $(WROOT)/mtl/config.mk
-include depend.mk
