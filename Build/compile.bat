@echo off
:: change working directory to the parent folder of this bat file, which is the root directory of this project
cd "%~dp0" && cd ..
"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvars64.bat" && "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64/cl.exe" /EHsc %1 %2