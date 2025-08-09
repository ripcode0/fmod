@echo off
setlocal ENABLEDELAYEDEXPANSION

REM --- usage check ---
if "%~1"=="" (
  echo Usage: %~nx0 path\to\libfoo.a
  exit /b 1
)

set "LIB=%~1"
if not exist "%LIB%" (
  echo [ERR] File not found: %LIB%
  exit /b 1
)

where ar >nul 2>nul || (
  echo [ERR] 'ar' not found. Install binutils (MSYS2/MinGW or LLVM).
  exit /b 1
)

REM --- work dir ---
set "WORK=%TEMP%\libscan-%RANDOM%%RANDOM%"
md "%WORK%" || (echo [ERR] Cannot create work dir & exit /b 1)
pushd "%WORK%"
copy "%LIB%" libtmp.a >nul

REM --- pick a member ---
ar t libtmp.a > members.txt || (echo [ERR] ar failed & goto :cleanup)

set "MEM="
for /f "usebackq delims=" %%M in ("members.txt") do (
  set "MEM=%%M"
  goto :gotmember
)

echo [ERR] No members in archive.
goto :cleanup

:gotmember
echo [*] Member: %MEM%
ar x libtmp.a "%MEM%" 2>nul

REM member 경로 보정 (thin archive 등 경로 포함 대비)
set "OBJ=%MEM%"
if not exist "%OBJ%" (
  for %%F in ("%OBJ%") do set "OBJ=%%~nxF"
)

if not exist "%OBJ%" (
  echo [ERR] Extracted object not found.
  goto :cleanup
)

REM --- try readelf / llvm-readobj / strings ---
where readelf >nul 2>nul
if %errorlevel%==0 (
  echo === readelf .comment ===
  readelf -p .comment "%OBJ%"
  goto :cleanup
)

where llvm-readobj >nul 2>nul
if %errorlevel%==0 (
  echo === llvm-readobj .comment ===
  llvm-readobj -p .comment "%OBJ%"
  goto :cleanup
)

where strings >nul 2>nul
if %errorlevel%==0 (
  echo === strings ^| findstr ===
  strings "%OBJ%" | findstr /i "gcc clang msvc apple clang"
  goto :cleanup
)

echo [ERR] Neither 'readelf', 'llvm-readobj', nor 'strings' was found.
echo       Install MSYS2/MinGW (binutils) or LLVM tools and retry.

:cleanup
popd
rd /s /q "%WORK%" >nul 2>&1
endlocal
