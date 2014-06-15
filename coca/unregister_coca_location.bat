@echo off
echo Unregistering COCA_LOCATION = %COCA_LOCATION%
REG DELETE HKEY_CURRENT_USER\Environment /v COCA_LOCATION /f
echo Broadcasting setting changed...
BroadcastSettingChanged.exe
pause
