@echo off
setlocal enabledelayedexpansion
setlocal enableextensions
echo.
echo AurorOS maintainers highly recommend you to switch to a Linux distribution
echo or to use WSL (Windows Subsystem for Linux) to build the system.
echo.
echo This batch script will attempt to build the system using WSL.
:choiceredo
echo.
echo Would you like to continue?
set /p "choice=[y/N] $ "
if "!choice!"=="y" goto :wslbuild
if "!choice!"=="Y" goto :wslbuild
if "!choice!"=="yes" goto :wslbuild
if "!choice!"=="YES" goto :wslbuild
endlocal
endlocal
exit /b

:wslbuild
wsl --cd "%cd%" -e bash -c "make"
endlocal
endlocal
exit /b