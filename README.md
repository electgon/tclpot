------------
#  TCLPOT
------------
Tclpot is a C API Wrapper for converting TCL scripts into executable binary. 
Tclpot is provided as free open source software. You can redistribute it 
and/or modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation, either version 3 of the 
License, or any later version.
This program is provided 'as-is' and it is distributed in the hope that it 
will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU General Public License for more details.

This repository can be obtained by:
```
git clone https://github.com/electgon/tclpot
```

To use this utility, Tclpot source is provided in tclpot.c. Build process is
better provided in a make file to conclude most needed build steps. Before
starting to wrap your TCL script make sure that you have compiled TCL source
files available in your machine. If you are not sure how to compile TCL source
files, please check www.electgon.com or check documentation distributed with
Tclpot source repository. 
To wrap your TCL script, basically you need to issue this command
In Windows:
```
nmake -f makefile.vc
```

In Linux:
```
make -f Makefile
```

But before that, open the make file and make sure that the following parameters are set correctly

* `C_WRAPPER`      : this is name of the tclpot.c file. If you renamed this c file, update this parameter accordingly.   
* `TCL_SCRIPT`     : to define name of user tcl script that needs to be wrapped (just the name without .tcl extension).
* `RULES_FILE`     : if you have rules file (.rc) mention its name here (just the name without .rc extension).
* `PROJECT`        : if your script is based on TCL scripts only (no gui), then make sure that this parameter is set to tcl.
                     Otherwise, if your script is based on Tk, set this parameter to tk.
* `TCL8_NAME`      : It was noticed that compiling TCL source code in Windows results in separate directory called 'tcl8' which
                     contains some old packages of TCL. In Linux, these packages are included in same main TCL packages directory.
                     So, In Linux no need for this parameter but In Windows it will be needed and usually user doesn't need to change it.
* `TCL_VER_NAME`   : this should be name of the main directory that contains basic TCL packages. i.e. the directory that contains init.tcl file. 
* `TK_VER_NAME`    : this should be name of the main directory that contains basic Tk packages. i.e. the directory that contains tk.tcl file. 
* `<pkg>_PKG_NAME` : If any other TCL package is needed within user TCL scrip, define a parameter with this needed package name.
* `PKGFLAGS`       : this parameter is used to pass previously mentioned package names to C source file which is using these names to define for
                     the TCL interpreter where to find these used packages. If you didn't add your target package into this flag, then you have
                     to provide packages names manually in the C source file.
* `TCL8_LIB_PATH`  : path to compiled tcl sources of version x which is applicable only in case of Windows as mentioned shortly.
* `TCL_LIB_PATH`   : path to compiled TCL sources of version x.x.
* `TK_LIB_PATH`    : path to compiled tk sources of version x.x.
* `TCL_PKG_<pkg>`  : if needed, provide path to user needed package <pkg> wich can be obtained from tcllib sources.
* `USE_RC_FILE`    : specify in this parameter whether you need to compile rc file or not.
* `RULES_INCLUDE`  : if you use Tk interface, you may need to include tk_base.rc file. mention here path to this file.
* `INCLUDE_DIR`    : TCL header files must be included during compile step. mention here path to include directory of TCL compiled source.
* `TCL_LIB_DIR`    : when you wrap your TCL with static libraries, mention here path to library directory that contains your TCL libs.
* `SYSTEM_LIBS`    : some general libraries of Windows or Linux are mentioned here. In principle no need to change this parameter.
* `TCL_LIBS`       : mention here name of TCL compiled libraries (shared or static). For example tcl87s.lib tclstub87.lib.
* `TK_LIBS`        : mention here name of Tk compiled libraries (shared or static). For example tk87s.lib tkstub87.lib.
* `CFLAGS`         : choose which macros are needed. Usually  "-DSTATIC_BUILD" will be needed
                     if your TCL script is using Tk interface, use "-DENABLE_TK"
                     if your TCL script is tcl only with input arguments (no Tk), use "-DCLI_ONLY_APP".
                     if you inlcuded tk_base.rc file and still you need to use your own icon, use "-DBASE_NO_TK_ICON"
                     "PKGFLAGS" is ued to pass name of user TCL script (TCL_SCRIPT) as well packages names to the wrapper, so that you can
                     avoid changing the wrapper source file.
* `COMPILE_FILES`  : if you have other C files that you need to compile, list here name of these files beside basic C_WRAPPER.
* `OBJECT_FILES`   : in most cases you will not need to modify this parameter.
* `ZIP_TOOL`       : define here path to your available zip program. For example "C:\Program Files\7-zip\7z.exe".
* `RULES_COMPILER` : if needed, define here rc compiler of your system.
* `COMPILER`       : define here C compiler of your system.

You can use `make clean` to clean up your build directory.

------------
## Repository Content

 - tclpot.c: main C code that is used to wrap user TCL script.
 - makefile.vc : make file used to build the executable for Windows systems.
 - Makefile :  make file used to build the executable for Linux systems.
 - user_rules : sample rules file in case of user is interested in adding icon to his executable.
 - tcl_inside.ico : default icon file to be used if rc file is enabled.
 - tk_hello.tcl  : example for a TCL script with Tk interface.
 - tclpot_guide.pdf : tutorial for understanding how to wrap a TCL with C to understand tclpot.c file.
 - README.md : this file.



