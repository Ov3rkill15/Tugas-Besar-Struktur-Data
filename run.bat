@echo off
echo Compiling...

REM Explicit file listing untuk kompatibilitas di semua Windows
g++ src/main.cpp src/login.cpp src/lowongan.cpp src/mahasiswa.cpp src/lamaran.cpp -Iinclude -o app.exe -lcomdlg32

if %errorlevel% neq 0 (
    echo Compilation Failed!
    pause
    exit /b %errorlevel%
)
echo Compilation Success! Running App...
echo ------------------------------------------------
app.exe
echo ------------------------------------------------
echo Cleaning up temporary files...
if exist search_results.txt del search_results.txt
echo Aplikasi ditutup.
pause
