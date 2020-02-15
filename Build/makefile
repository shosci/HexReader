# makefile for nmake to automate the build

# caller of nmake should define env var ROOTDIR properly
BIN=$(ROOTDIR)\Bin
OBJ=$(ROOTDIR)\Obj

all: prep $(BIN)\HexReader.exe

$(OBJ)\main.obj : $(ROOTDIR)\src\main.cpp
	cl -c -Zi -EHsc -Fo$(OBJ)\ $**

$(OBJ)\DiskUtil.obj : $(ROOTDIR)\src\DiskUtil.cpp
	cl -c -Zi -EHsc -Fo$(OBJ)\ $**

$(OBJ)\HexPrinter.obj : $(ROOTDIR)\src\HexPrinter.cpp
	cl -c -Zi -EHsc -Fo$(OBJ)\ $**

$(OBJ)\DiskReader.obj : $(ROOTDIR)\src\DiskReader.cpp
	cl -c -Zi -EHsc -Fo$(OBJ)\ $**

$(BIN)\HexReader.exe : $(OBJ)\main.obj $(OBJ)\DiskUtil.obj $(OBJ)\HexPrinter.obj $(OBJ)\DiskReader.obj
  link -debug -out:$@ $**

# create build dirs
$(OBJ) :
	md $(OBJ)

$(BIN) :
	md $(BIN)

prep : $(OBJ) $(BIN)
