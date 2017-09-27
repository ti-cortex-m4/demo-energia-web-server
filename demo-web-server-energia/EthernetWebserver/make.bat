@echo off
setLocal EnableDelayedExpansion

for /f "tokens=* delims= " %%a in (1.html) do (
echo   client.println(^"%%a^"^); >> 2.html
)