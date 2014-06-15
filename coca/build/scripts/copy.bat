@echo off
if not exist %2 mkdir %2
xcopy %1 %2 /Y