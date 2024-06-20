@ECHO OFF

cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tp *.c /link /OUT:implant.exe /SUBSYSTEM:CONSOLE /MACHINE:x64

del *.obj
pause