@echo off
echo Compiling...
g++ src/*.cpp -Iinclude -o app.exe -lcomdlg32
if %errorlevel% neq 0 (
    echo Compilation Failed!
    pause
    exit /b %errorlevel%
)
echo Compilation Success! Running App...
echo ------------------------------------------------
app.exe
pause
