# makefile for nmake to automate the build

# caller of nmake should define env var ROOTDIR properly
BIN=$(ROOTDIR)\Bin
OBJ=$(ROOTDIR)\Obj

all: prep $(BIN)\HexReader.exe

$(OBJ)\main.obj : $(ROOTDIR)\Program\main.cpp
	cl -c -Zi -EHsc -Fo$(OBJ)\ $**

$(OBJ)\DiskUtil.obj : $(ROOTDIR)\Util\DiskUtil.cpp
	cl -c -Zi -EHsc -Fo$(OBJ)\ $**

$(OBJ)\HexPrinter.obj : $(ROOTDIR)\Util\HexPrinter.cpp
	cl -c -Zi -EHsc -Fo$(OBJ)\ $**

$(BIN)\HexReader.exe : $(OBJ)\main.obj $(OBJ)\DiskUtil.obj $(OBJ)\HexPrinter.obj
  link -debug -out:$@ $**

# create build dirs
$(OBJ) :
	md $(OBJ)

$(BIN) :
	md $(BIN)

prep : $(OBJ) $(BIN)
