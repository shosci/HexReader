@echo off

:: set up build env variables
call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvars64.bat"

:: set ROOTDIR env var and it will be available in makefile as a macro
::  %0 is the bat itself, %~dp0 gives the drive and path of the bat 
set ROOTDIR=%~dp0
::  Trim the '\Build\' part to get the root folder which should be ROOTDIR="D:\Projects\HexReader"
set ROOTDIR=%ROOTDIR:\Build\=%

:: Change current working directory
cd %ROOTDIR%\Build

:: call nmake to build
nmake.exe