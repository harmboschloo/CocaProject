@echo off
echo Registering COCA_LOCATION = %cd%
REG ADD HKEY_CURRENT_USER\Environment /v COCA_LOCATION /d "%cd%" /f
echo Broadcasting setting changed...
BroadcastSettingChanged.exe
pause