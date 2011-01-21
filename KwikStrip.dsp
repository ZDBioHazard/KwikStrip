# Microsoft Developer Studio Project File - Name="KwikStrip" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# TARGTYPE "Win32 (x86) Application" 0x0101
CFG=KWIKSTRIP - WIN32
!MESSAGE "KwikStrip - Win32" (based on "Win32 (x86) Application")

# Begin Project
# PROP Intermediate_Dir "build"
# PROP Output_Dir "release"
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /map:"build/KwikStrip.map" /machine:I386
# SUBTRACT LINK32 /pdb:none /debug /nodefaultlib /force

# Begin Target
# Name "KwikStrip - Win32"

# Begin Group "Resources"
	# Begin Source File
SOURCE=.\bmp_about.bmp
	# End Source File
	# Begin Source File
SOURCE=.\bmp_gfx.bmp
	# End Source File
	# Begin Source File
SOURCE=.\ico_icon.ico
	# End Source File
# End Group

# Begin Group "Source"
	# Begin Source File
SOURCE=.\biolib.c
	# End Source File
	# Begin Source File
SOURCE=.\cfg.c
	# End Source File
	# Begin Source File
SOURCE=.\crash.c
	# End Source File
	# Begin Source File
SOURCE=.\kwik.c
	# End Source File
	# Begin Source File
SOURCE=.\strip.c
	# End Source File
	# End Group
	# Begin Source File
SOURCE=.\kwik.h
	# End Source File
	# Begin Source File
SOURCE=.\kwik.rc
	# End Source File
# End Group

# End Target
# End Project
