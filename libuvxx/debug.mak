#Generated by VisualGDB (http://visualgdb.com)
#DO NOT EDIT THIS FILE MANUALLY UNLESS YOU ABSOLUTELY NEED TO
#USE VISUALGDB PROJECT PROPERTIES DIALOG INSTEAD

BINARYDIR := Debug

#Toolchain
CC := C:/SysGCC/linaro/bin/arm-linux-gnueabihf-gcc.exe
CXX := C:/SysGCC/linaro/bin/arm-linux-gnueabihf-g++.exe
LD := $(CXX)
AR := C:/SysGCC/linaro/bin/arm-linux-gnueabihf-ar.exe
OBJCOPY := C:/SysGCC/linaro/bin/arm-linux-gnueabihf-objcopy.exe

#Additional flags
PREPROCESSOR_MACROS := DEBUG __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
INCLUDE_DIRS := include include/internal ../libraries/libuv/include
LIBRARY_DIRS := 
LIBRARY_NAMES := pthread
ADDITIONAL_LINKER_INPUTS := 
MACOS_FRAMEWORKS := 
LINUX_PACKAGES := 

CFLAGS := -ggdb -ffunction-sections -O0 -std=c++1y -Wno-unused-parameter -Wextra -fno-common -pthread -D_DEBUG -fexceptions -fnon-call-exceptions -D_NO_ASYNCRTIMP -Wall  -Wno-unknown-pragmas
CXXFLAGS := -ggdb -ffunction-sections -O0 -std=c++1y -Wno-unused-parameter -Wextra -fno-common -pthread -D_DEBUG -fexceptions -fnon-call-exceptions -D_NO_ASYNCRTIMP -Wall  -Wno-unknown-pragmas
ASFLAGS := 
LDFLAGS := -Wl,-gc-sections
COMMONFLAGS := 

START_GROUP := -Wl,--start-group
END_GROUP := -Wl,--end-group

#Additional options detected from testing the toolchain
USE_DEL_TO_CLEAN := 1
IS_LINUX_PROJECT := 1
