#----------------------------------
# Copyright (c) 2020 Electgon
#----------------------------------
#
# General usage:
#   make -f Makefile
#----------------------------------


C_WRAPPER  = tclpot
TCL_SCRIPT = tk_hello
RULES_FILE = user_rules


#------------------- TCL only or with TK-------------
#  please choose below if it is tcl only application
#  or tcl with tk

PROJECT = tcl
# PROJECT = tk
#----------------------------------------------------


#------------- Used Libraries and Packages---------------
TCL8_NAME    = tcl8
TCL_VER_NAME = tcl8.7
TK_VER_NAME  = tk8.7
CMDLINE_PKG_NAME = cmdline
STRUCT_PKG_NAME = struct

# --- If you use other package or module, you can pass its name to the
# --- C code like the following flags, or add it manually in the 
# --- C source code file.
PKGFLAGS = \
           -D SCRIPT_TO_RUN=$(TCL_SCRIPT) \
           -D MK_TCL8_NAME=$(TCL8_NAME) \
           -D MK_STR_TCL_VERSION=$(TCL_VER_NAME) \
           -D MK_STR_TK_VERSION=$(TK_VER_NAME) \
           -D MK_CMDLINE_PKG=$(CMDLINE_PKG_NAME) \
           -D MK_STRUCT_PKG=$(STRUCT_PKG_NAME) \
#----------------------------------------------------

BASE_BUILD_PATH = /home/pi/TCL_Build
#------------ Path to source of used Libraries and Packages---------------
#TCL8_LIB_PATH  = "$(BASE_BUILD_PATH)/lib/"$(TCL8_NAME)
TCL_LIB_PATH    = "$(BASE_BUILD_PATH)/lib/tcl_library"
TK_LIB_PATH     = "$(BASE_BUILD_PATH)/lib/"$(TK_VER_NAME)
TCL_PKG_CMDLINE = "$(BASE_BUILD_PATH)/lib/tcllib-1.20/modules/"$(CMDLINE_PKG_NAME)
TCL_PKG_STRUCT  = "$(BASE_BUILD_PATH)/lib/tcllib-1.20/modules/"$(STRUCT_PKG_NAME)
#----------------------------------------------------


#------------ If you don't use Rules file, set USE_RC_FILE = 0 ---------------
USE_RC_FILE = 0
RULES_INCLUDE = -I"/path/to/rc"
#----------------------------------------------------


#------------ Path to compiled TCL sources ---------------
INCLUDE_DIR = -I$(BASE_BUILD_PATH)/include
TCL_LIB_DIR = $(BASE_BUILD_PATH)/lib
#----------------------------------------------------
ifeq ($(PROJECT), tcl)
SYSTEM_LIBS = -lz -ldl -lm -lpthread
else 
ifeq ($(PROJECT), tk)
SYSTEM_LIBS = -lz -ldl -lm -lpthread -lX11 -lXft -lfontconfig -lXss
endif
endif
TCL_LIBS = $(TCL_LIB_DIR)/libtcl8.7.a $(TCL_LIB_DIR)/libtclstub8.7.a
TK_LIBS =  $(TCL_LIB_DIR)/libtk8.7.a  $(TCL_LIB_DIR)/libtkstub8.7.a
#----------------------------------------------------

ifeq ($(PROJECT), tcl)
CFLAGS = -DSTATIC_BUILD  -DBASE_NO_TK_ICON -DCLI_ONLY_APP $(PKGFLAGS)
else 
ifeq ($(PROJECT), tk)
CFLAGS = -DSTATIC_BUILD  -DBASE_NO_TK_ICON -DENABLE_TK $(PKGFLAGS)
endif
endif

COMPILE_FILES = $(C_WRAPPER).c
ifeq ($(USE_RC_FILE), 1)
OBJECT_FILES = $(C_WRAPPER).o $(RULES_FILE).res
else
OBJECT_FILES = $(C_WRAPPER).o
endif


ZIP_TOOL = zip
ZIPPED_TCL_FILE  = zipped_tcl_lib
UNZIPPED_TCL_LIB = TCL_RUNTIME_LIB
EXE =

# Compilers
RULES_COMPILER = windres
COMPILER = gcc

# other general utilities
RM    = rm -f
RMDIR = rm -rf
MKDIR = mkdir
ECHO  = echo
STRIP = strip
COPY  = cp
CPDIR = cp -r
CD    = cd
CAT   = cat



#---------------------------------------------------------------------
# Building
#---------------------------------------------------------------------

all: wrapper_build

wrapper_build:
	$(RM) $(ZIPPED_TCL_FILE).zip
	$(RMDIR) $(UNZIPPED_TCL_LIB)
	$(MKDIR) $(UNZIPPED_TCL_LIB)
	$(ECHO) Building executable for user TCL script ....
	@if ($(USE_RC_FILE) == 1); then $(RULES_COMPILER) $(CFLAGS) $(RULES_INCLUDE) $(RULES_FILE).rc; fi
	$(COMPILER) $(INCLUDE_DIR) $(CFLAGS) -c $(COMPILE_FILES) 
	$(COMPILER) $(OBJECT_FILES) -o $(TCL_SCRIPT)$(EXE) $(TCL_LIBS) $(TK_LIBS) $(SYSTEM_LIBS)
	$(STRIP) $(TCL_SCRIPT)$(EXE)
	$(COPY) $(TCL_SCRIPT).tcl $(UNZIPPED_TCL_LIB)
	$(CPDIR) $(TCL_LIB_PATH) $(UNZIPPED_TCL_LIB)/$(TCL_VER_NAME)
	$(CPDIR) $(TK_LIB_PATH) $(UNZIPPED_TCL_LIB)
	$(CPDIR) $(TCL_PKG_CMDLINE) $(UNZIPPED_TCL_LIB)
	$(CPDIR) $(TCL_PKG_STRUCT) $(UNZIPPED_TCL_LIB)
	cd $(UNZIPPED_TCL_LIB);	$(ZIP_TOOL) -r -q ../$(ZIPPED_TCL_FILE).zip *;	cd ..
	$(CAT) $(ZIPPED_TCL_FILE).zip >> $(TCL_SCRIPT)$(EXE)

#---------------------------------------------------------------------
# Clean up
#---------------------------------------------------------------------
clean:
	$(RM) -f *.o $(TCL_SCRIPT)$(EXE) $(ZIPPED_TCL_FILE).zip
	$(RM) -rf $(UNZIPPED_TCL_LIB)

