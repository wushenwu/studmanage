# Microsoft Developer Studio Generated NMAKE File, Based on studmanage.dsp
!IF "$(CFG)" == ""
CFG=studmanage - Win32 Debug
!MESSAGE No configuration specified. Defaulting to studmanage - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "studmanage - Win32 Release" && "$(CFG)" != "studmanage - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "studmanage.mak" CFG="studmanage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "studmanage - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "studmanage - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "studmanage - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\studmanage.exe"


CLEAN :
	-@erase "$(INTDIR)\file.obj"
	-@erase "$(INTDIR)\fit.obj"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\studmanage.obj"
	-@erase "$(INTDIR)\table.obj"
	-@erase "$(INTDIR)\tool.obj"
	-@erase "$(INTDIR)\ui.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\studmanage.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\studmanage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\studmanage.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\studmanage.pdb" /machine:I386 /out:"$(OUTDIR)\studmanage.exe" 
LINK32_OBJS= \
	"$(INTDIR)\file.obj" \
	"$(INTDIR)\fit.obj" \
	"$(INTDIR)\studmanage.obj" \
	"$(INTDIR)\table.obj" \
	"$(INTDIR)\tool.obj" \
	"$(INTDIR)\ui.obj" \
	"$(INTDIR)\log.obj"

"$(OUTDIR)\studmanage.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "studmanage - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\studmanage.exe" "$(OUTDIR)\studmanage.bsc"


CLEAN :
	-@erase "$(INTDIR)\file.obj"
	-@erase "$(INTDIR)\file.sbr"
	-@erase "$(INTDIR)\fit.obj"
	-@erase "$(INTDIR)\fit.sbr"
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\studmanage.obj"
	-@erase "$(INTDIR)\studmanage.sbr"
	-@erase "$(INTDIR)\table.obj"
	-@erase "$(INTDIR)\table.sbr"
	-@erase "$(INTDIR)\tool.obj"
	-@erase "$(INTDIR)\tool.sbr"
	-@erase "$(INTDIR)\ui.obj"
	-@erase "$(INTDIR)\ui.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\studmanage.bsc"
	-@erase "$(OUTDIR)\studmanage.exe"
	-@erase "$(OUTDIR)\studmanage.ilk"
	-@erase "$(OUTDIR)\studmanage.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\studmanage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\studmanage.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\file.sbr" \
	"$(INTDIR)\fit.sbr" \
	"$(INTDIR)\studmanage.sbr" \
	"$(INTDIR)\table.sbr" \
	"$(INTDIR)\tool.sbr" \
	"$(INTDIR)\ui.sbr" \
	"$(INTDIR)\log.sbr"

"$(OUTDIR)\studmanage.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\studmanage.pdb" /debug /machine:I386 /out:"$(OUTDIR)\studmanage.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\file.obj" \
	"$(INTDIR)\fit.obj" \
	"$(INTDIR)\studmanage.obj" \
	"$(INTDIR)\table.obj" \
	"$(INTDIR)\tool.obj" \
	"$(INTDIR)\ui.obj" \
	"$(INTDIR)\log.obj"

"$(OUTDIR)\studmanage.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("studmanage.dep")
!INCLUDE "studmanage.dep"
!ELSE 
!MESSAGE Warning: cannot find "studmanage.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "studmanage - Win32 Release" || "$(CFG)" == "studmanage - Win32 Debug"
SOURCE=.\file.c

!IF  "$(CFG)" == "studmanage - Win32 Release"


"$(INTDIR)\file.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "studmanage - Win32 Debug"


"$(INTDIR)\file.obj"	"$(INTDIR)\file.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fit.c

!IF  "$(CFG)" == "studmanage - Win32 Release"


"$(INTDIR)\fit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "studmanage - Win32 Debug"


"$(INTDIR)\fit.obj"	"$(INTDIR)\fit.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\log.c

!IF  "$(CFG)" == "studmanage - Win32 Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "studmanage - Win32 Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\studmanage.c

!IF  "$(CFG)" == "studmanage - Win32 Release"


"$(INTDIR)\studmanage.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "studmanage - Win32 Debug"


"$(INTDIR)\studmanage.obj"	"$(INTDIR)\studmanage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\table.c

!IF  "$(CFG)" == "studmanage - Win32 Release"


"$(INTDIR)\table.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "studmanage - Win32 Debug"


"$(INTDIR)\table.obj"	"$(INTDIR)\table.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\tool.c

!IF  "$(CFG)" == "studmanage - Win32 Release"


"$(INTDIR)\tool.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "studmanage - Win32 Debug"


"$(INTDIR)\tool.obj"	"$(INTDIR)\tool.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ui.c

!IF  "$(CFG)" == "studmanage - Win32 Release"


"$(INTDIR)\ui.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "studmanage - Win32 Debug"


"$(INTDIR)\ui.obj"	"$(INTDIR)\ui.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

