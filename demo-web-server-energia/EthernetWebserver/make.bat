@echo off
setLocal EnableDelayedExpansion

if exist index.c del /F index.c

for /f "tokens=* delims= " %%a in (index.html) do (
echo   client.println(^"%%a^"^); >> index.c
)

if exist config.c del /F config.c

for /f "tokens=* delims= " %%a in (config.html) do (
echo   client.println(^"%%a^"^); >> config.c
)