@echo off
setlocal enabledelayedexpansion

rem Text-only Gradle launcher for this repository.
rem The binary Gradle wrapper JAR is intentionally not tracked.

set "APP_HOME=%~dp0"
set "PROPERTIES_FILE=%APP_HOME%gradle\wrapper\gradle-wrapper.properties"

if not exist "%PROPERTIES_FILE%" (
  echo Gradle wrapper properties not found: %PROPERTIES_FILE% 1>&2
  exit /b 1
)

for /f "usebackq delims=" %%U in (`powershell -NoProfile -ExecutionPolicy Bypass -Command "(Get-Content '%PROPERTIES_FILE%' | Where-Object { $_ -like 'distributionUrl=*' } | Select-Object -First 1) -replace '^distributionUrl=', '' -replace '\\:', ':'"`) do set "DISTRIBUTION_URL=%%U"

if "%DISTRIBUTION_URL%"=="" (
  echo distributionUrl is missing in %PROPERTIES_FILE% 1>&2
  exit /b 1
)

if "%GRADLE_USER_HOME%"=="" set "GRADLE_USER_HOME=%USERPROFILE%\.gradle"

for %%F in ("%DISTRIBUTION_URL%") do set "ZIP_NAME=%%~nxF"
set "DIST_NAME=%ZIP_NAME:.zip=%"
set "DIST_DIR=%GRADLE_USER_HOME%\wrapper\dists\%DIST_NAME%"
set "ZIP_FILE=%DIST_DIR%\%ZIP_NAME%"
set "GRADLE_BIN=%DIST_DIR%\%DIST_NAME%\bin\gradle.bat"

if not exist "%GRADLE_BIN%" (
  if not exist "%DIST_DIR%" mkdir "%DIST_DIR%"
  if not exist "%ZIP_FILE%" (
    echo Downloading %DISTRIBUTION_URL% 1>&2
    powershell -NoProfile -ExecutionPolicy Bypass -Command "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '%DISTRIBUTION_URL%' -OutFile '%ZIP_FILE%'"
    if errorlevel 1 exit /b 1
  )
  powershell -NoProfile -ExecutionPolicy Bypass -Command "if (Test-Path '%DIST_DIR%\.tmp') { Remove-Item -Recurse -Force '%DIST_DIR%\.tmp' }; New-Item -ItemType Directory -Path '%DIST_DIR%\.tmp' | Out-Null; Expand-Archive -Force '%ZIP_FILE%' '%DIST_DIR%\.tmp'; if (Test-Path '%DIST_DIR%\%DIST_NAME%') { Remove-Item -Recurse -Force '%DIST_DIR%\%DIST_NAME%' }; Move-Item '%DIST_DIR%\.tmp\%DIST_NAME%' '%DIST_DIR%\%DIST_NAME%'; Remove-Item -Force '%DIST_DIR%\.tmp'"
  if errorlevel 1 exit /b 1
)

call "%GRADLE_BIN%" %*
exit /b %ERRORLEVEL%
