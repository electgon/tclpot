// -----------------------------------------------------------------------------------------
// |      ____________     | Copyright (C) 2020 - Electgon - www.electgon.com
// |     /            \    | This source file is free: you can redistribute it and/or  
// |    /    ______    \   | modify it under the terms of the GNU General Public License 
// |   /    /      \    \  | as published by the Free Software Foundation, either 
// |  /    /________\    \ | version 3 of the License, or any later version.
// | (     _______________)| This program is distributed 'as-is' in the hope that it will be 
// |  \    \        ,----, | useful, but WITHOUT ANY WARRANTY; without even the implied
// |   \    \______/    /  | warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
// |    \              /   | PURPOSE. See the GNU General Public License for more details.
// |     \____________/    |              -------------------------
// |                       | Design: tclpot C wrapper
// |                       | Author: Electgon
// |                       | Version:
// |                       | Build Date: 14.11.2020
// -----------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>
#include <tk.h>


#ifdef APP_4_WIN
  #define app_drive "zipfs:"
#else
  #define app_drive "//zipfs:"
#endif
#define app_mount "/appfs"
#define CMD_MAX_SIZE 256
#define STR(x) #x
#define STR_2(x) STR(x)
// the following macros get its values from the calling Makefile
#define STR_SCR_MACRO   STR_2(SCRIPT_TO_RUN)
#define STR_TCL_VERSION STR_2(MK_STR_TCL_VERSION)
#define STR_TK_VERSION  STR_2(MK_STR_TK_VERSION)
#define STR_USR_PKG_1   STR_2(MK_CMDLINE_PKG)
#define STR_USR_PKG_2   STR_2(MK_STRUCT_PKG)


#define add_pkg_path(x)  sprintf(pkg_autopath, app_drive app_mount "/" x);\
                         Tcl_SetVar(myinterp, "::auto_path", pkg_autopath, TCL_APPEND_VALUE | TCL_LIST_ELEMENT);

#define TCL_SOURCE_CMD "source " app_drive  app_mount "/" STR_SCR_MACRO ".tcl"

static const char *user_script = TCL_SOURCE_CMD;




//  // Microsoft deprecated vsprintf, so in doubt use the following alternative
//
//  len = _vscprintf( arg_str, ap ) + 1; // +1 for terminating '\0'
//  cmd_str = (char *) malloc( len * sizeof(char) );
//
//  if ( NULL != cmd_str ) {
//      vsprintf_s(cmd_str, len, arg_str, ap);
//      result = Tcl_Eval(interp,cmd_str);
//  } else {
//    printf("C: ERROR: Couldn't allocate length of TCL command \n");
//  }


/*
** Execute Tcl command.
*/
int exe_tcl_cmd(Tcl_Interp *interp, const char *arg_str, ...){   
  
  char *cmd_str = Tcl_Alloc(CMD_MAX_SIZE);   
  va_list ap;   
  int result=0;   
  va_start(ap,arg_str);   
  vsprintf(cmd_str, arg_str, ap);   
  result = Tcl_Eval(interp,cmd_str);   
  Tcl_Free(cmd_str);
  va_end(ap);
   
return result; 
}

int main (int argc ,char *argv[])
{
char tcl_dir[CMD_MAX_SIZE];
char tk_dir[CMD_MAX_SIZE];
char pkg_autopath[CMD_MAX_SIZE];
char scr_argc[CMD_MAX_SIZE];
char *app_args;


static char wait_x[] = 
    "bind . <Destroy> {+if {\"%W\"==\".\"} exit}\n"
    "while 1 {vwait forever}";
    Tcl_FindExecutable(argv[0]);
    Tcl_Interp *myinterp;
    printf ("C: Starting ... \n");
    myinterp = Tcl_CreateInterp();

#ifdef CLI_ONLY_APP
    app_args = Tcl_Merge(argc-1, argv+1);
    // the result string is dynamically allocated using Tcl_Alloc; 
    // the caller must eventually release the space using Tcl_Free.
    Tcl_SetVar(myinterp, "argv", app_args, TCL_GLOBAL_ONLY);
    Tcl_Free(app_args);
    sprintf(scr_argc, "%d", argc-1);
    Tcl_SetVar(myinterp, "argc", scr_argc, TCL_GLOBAL_ONLY);
    Tcl_SetVar(myinterp, "argv0", argv[0], TCL_GLOBAL_ONLY);
    Tcl_SetVar(myinterp, "tcl_interactive", "0", TCL_GLOBAL_ONLY);
#endif

    if (TclZipfs_Init(myinterp) != TCL_OK) {
        printf("Error in initializing Virtual File System\n");
        return TCL_ERROR;
    }
    if ( TclZipfs_Mount(myinterp, app_mount, Tcl_GetNameOfExecutable(), NULL) != TCL_OK) {
        printf("Error in mounting Virtual File System\n");
        return TCL_ERROR;
    }
    sprintf(tcl_dir, "%s%s/%s", app_drive, app_mount, STR_TCL_VERSION);
    Tcl_SetVar2(myinterp, "env", "TCL_LIBRARY", tcl_dir, TCL_GLOBAL_ONLY);
    Tcl_SetVar(myinterp,"tcl_library",tcl_dir,TCL_GLOBAL_ONLY);
    Tcl_SetVar(myinterp,"tcl_libPath",tcl_dir,TCL_GLOBAL_ONLY);
#ifdef ENABLE_TK
    sprintf(tk_dir, "%s%s/%s", app_drive, app_mount, STR_TK_VERSION);
    Tcl_SetVar2(myinterp, "env", "TK_LIBRARY", tk_dir, TCL_GLOBAL_ONLY);
    Tcl_SetVar(myinterp, "tk_library", tk_dir, TCL_GLOBAL_ONLY);
#endif


    if (Tcl_Init(myinterp) != TCL_OK) {
        printf("Error: %s\n",Tcl_GetStringResult(myinterp));
        return TCL_ERROR;
    }
#ifdef ENABLE_TK
    if (Tk_Init(myinterp) != TCL_OK ){
        printf("Error: %s\n",Tcl_GetStringResult(myinterp));
        return TCL_ERROR;
    }
    Tcl_StaticPackage(myinterp,"Tk", Tk_Init, 0);
    //Tk_InitConsoleChannels(myinterp);
    //Tk_CreateConsoleWindow(myinterp);
#endif

    // other user packages or modules
    add_pkg_path(STR_USR_PKG_1);
    add_pkg_path(STR_USR_PKG_2);


    if (exe_tcl_cmd(myinterp, user_script) != TCL_OK) {
        printf("Error: %s\n",Tcl_GetStringResult(myinterp));
        return TCL_ERROR;
    }
#ifdef ENABLE_TK
    Tcl_Eval(myinterp,wait_x);
#endif
    
   Tcl_DeleteInterp(myinterp);
   Tcl_Finalize();
   printf ("C: Finished\n");

    return TCL_OK;
}