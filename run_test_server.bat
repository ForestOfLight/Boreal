@echo off
setlocal

:: ============================================================
:: Configuration
:: ============================================================
set BUILD_CONFIG=RelWithDebInfo
set SERVER_DIR=
:: ============================================================

:: PROJECT_DIR is the folder containing this script
set PROJECT_DIR=%~dp0
set PROJECT_DIR=%PROJECT_DIR:~0,-1%

:: Default SERVER_DIR to PROJECT_DIR\test_server if not overridden above
if not defined SERVER_DIR set SERVER_DIR=%PROJECT_DIR%\test_server

set DLL_SRC=%PROJECT_DIR%\cmake-build-relwithdebinfo\%BUILD_CONFIG%\endstone_boreal.dll
set VENV_DIR=%SERVER_DIR%\venv
set PLUGINS_DIR=%SERVER_DIR%\plugins

:: Read endstone version from CMakeLists.txt
for /f "tokens=3 delims=() " %%v in ('findstr /C:"set(ENDSTONE_VERSION" "%PROJECT_DIR%\CMakeLists.txt"') do set ENDSTONE_VERSION=%%v
if not defined ENDSTONE_VERSION (
    echo ERROR: Could not read ENDSTONE_VERSION from CMakeLists.txt
    exit /b 1
)

:: Bootstrap venv on first run
if not exist "%VENV_DIR%\Scripts\activate.bat" (
    echo [0/2] Creating test server venv ^(endstone==%ENDSTONE_VERSION%^)...
    python -m venv "%VENV_DIR%"
    call "%VENV_DIR%\Scripts\activate.bat"
    pip install --quiet endstone==%ENDSTONE_VERSION%
    if errorlevel 1 (
        echo ERROR: Failed to install endstone==%ENDSTONE_VERSION%
        exit /b 1
    )
) else (
    call "%VENV_DIR%\Scripts\activate.bat"
)

:: Kill any existing server instance running from this test server directory
for /f "tokens=*" %%p in ('powershell -NoProfile -Command "Get-Process bedrock_server -ErrorAction SilentlyContinue | Where-Object { $_.Path -like '%SERVER_DIR%\*' } | Select-Object -ExpandProperty Id"') do (
    echo Stopping existing server instance ^(PID %%p^)...
    taskkill /F /PID %%p >nul 2>&1
    powershell -NoProfile -Command "Wait-Process -Id %%p -ErrorAction SilentlyContinue"
)

:: Ensure plugins directory exists
if not exist "%PLUGINS_DIR%" (
    echo Creating test server plugins directory...
    mkdir "%PLUGINS_DIR%"
)

echo [1/2] Copying DLL to test server plugins...
copy /Y "%DLL_SRC%" "%PLUGINS_DIR%\"
if errorlevel 1 (
    echo ERROR: Failed to copy DLL.
    echo   Expected: %DLL_SRC%
    exit /b 1
)

echo [2/2] Starting Endstone test server...
cd /d "%SERVER_DIR%\.."
endstone -s ".\test_server"
