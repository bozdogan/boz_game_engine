@ECHO OFF

REM /DDEBUG
SETLOCAL
set COMPFLAG=/Zi /FC /EHsc /DDEBUG /I "%~dp0\include"
set LINKFLAG=/LIBPATH:"%~dp0\lib\x64" Shell32.lib /SUBSYSTEM:CONSOLE
set BUILDDIR=\build

IF NOT EXIST "%BUILDDIR%" mkdir "%BUILDDIR%"
PUSHD %BUILDDIR%
cl %COMPFLAG% "%~dp0\code\win32_game_engine.cpp" /link %LINKFLAG% SDL2.lib SDL2main.lib || GOTO FAIL
POPD

IF NOT EXIST "%BUILDDIR%\SDL2.dll" copy "%~dp0\lib\x64\SDL2.dll" "%BUILDDIR%"
GOTO END

:FAIL
POPD && EXIT /B 1

:END
ENDLOCAL