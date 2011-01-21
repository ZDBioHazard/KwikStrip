# Microsoft Developer Studio Project File - Name="SkinEd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# TARGTYPE "Win32 (x86) Application" 0x0101
CFG=SKINED - WIN32
!MESSAGE "SkinEd - Win32" (based on "Win32 (x86) Application")

# Begin Project
# PROP Intermediate_Dir "build"
# PROP Output_Dir "release"
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib shell32.lib /nologo /subsystem:windows /map:"build/SkinEd.map" /machine:I386
# SUBTRACT LINK32 /pdb:none /debug /nodefaultlib /force

# Begin Target
	# Name "SkinEd - Win32"

	# Begin Source File
SOURCE=.\bmp_about.bmp
	# End Source File
	# Begin Source File
SOURCE=.\skin.c
	# End Source File
	# Begin Source File
SOURCE=.\skin.h
	# End Source File
	# Begin Source File
SOURCE=.\skin.rc
	# End Source File
# End Target

# End Project
