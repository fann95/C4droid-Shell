# this one is important
set( CMAKE_SYSTEM_NAME Linux )
# this one not so much
set( CMAKE_SYSTEM_VERSION 1 )
# rpath makes low sence for Android
set( CMAKE_SKIP_RPATH TRUE CACHE BOOL "If set, runtime paths are not added when using shared libraries." )
set( CMAKE_HOST_UNIX 1 )
set(  UNIX 1)

#i686 mips armv6 armv7-a
set( CMAKE_SYSTEM_PROCESSOR "armv5te" )
if( CMAKE_BINARY_DIR AND EXISTS "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeSystem.cmake" )
 # really dirty hack
 # it is not possible to change CMAKE_SYSTEM_PROCESSOR after the first run...
 file( APPEND "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeSystem.cmake" "SET(CMAKE_SYSTEM_PROCESSOR \"${CMAKE_SYSTEM_PROCESSOR}\")\n" )
endif()

#setup path
set( C4DROID_DIR /data/data/com.n0n3m4.droidc )
set( MACHINE_NAME arm-linux-androideabi CACHE STRING "MACHINE_NAME")
set( GCC_ROOT /data/data/com.n0n3m4.droidc/files/gcc CACHE PATH "path where installed gcc")

set( CMAKE_INSTALL_PREFIX "${C4DROID_DIR}/usr" CACHE STRING "path for installing" )
# where is the target environment
set( CMAKE_FIND_ROOT_PATH "${GCC_ROOT}/bin" "${GCC_ROOT}/${MACHINE_NAME}" "${CMAKE_INSTALL_PREFIX}" "${CMAKE_INSTALL_PREFIX}/share" )
set( CMAKE_C_COMPILER   "${GCC_ROOT}/bin/${MACHINE_NAME}-gcc"     CACHE PATH "C compiler")
set( CMAKE_CXX_COMPILER "${GCC_ROOT}/bin/${MACHINE_NAME}-g++"     CACHE PATH "C++ compiler")
set( CMAKE_ASM_COMPILER "${GCC_ROOT}/bin/${MACHINE_NAME}-gcc"     CACHE PATH "assembler" )
set( CMAKE_STRIP        "${GCC_ROOT}/bin/${MACHINE_NAME}-strip"   CACHE PATH "strip" )
set( CMAKE_AR           "${GCC_ROOT}/bin/${MACHINE_NAME}-ar"      CACHE PATH "archive" )
set( CMAKE_LINKER       "${GCC_ROOT}/bin/${MACHINE_NAME}-ld"      CACHE PATH "linker" )
set( CMAKE_NM           "${GCC_ROOT}/bin/${MACHINE_NAME}-nm"      CACHE PATH "nm" )
set( CMAKE_OBJCOPY      "${GCC_ROOT}/bin/${MACHINE_NAME}-objcopy" CACHE PATH "objcopy" )
set( CMAKE_OBJDUMP      "${GCC_ROOT}/bin/${MACHINE_NAME}-objdump" CACHE PATH "objdump" )
set( CMAKE_RANLIB       "${GCC_ROOT}/bin/${MACHINE_NAME}-ranlib"  CACHE PATH "ranlib" )
set( QT_SEARCH_PATH "${GCC_ROOT}/qt" CACHE PATH "Path to Qt" )
# Force set compilers because standard identification works badly for us
include( CMakeForceCompiler )
CMAKE_FORCE_C_COMPILER( "${CMAKE_C_COMPILER}" GNU )
set( CMAKE_C_PLATFORM_ID Linux )
set( CMAKE_C_SIZEOF_DATA_PTR 4 )
set( CMAKE_C_COMPILER_ABI ELF )
CMAKE_FORCE_CXX_COMPILER( "${CMAKE_CXX_COMPILER}" GNU )
set( CMAKE_CXX_PLATFORM_ID Linux )
set( CMAKE_CXX_SIZEOF_DATA_PTR 4 )
set( CMAKE_CXX_COMPILER_ABI ELF )
set( CMAKE_CXX_SOURCE_FILE_EXTENSIONS cc cp cxx cpp CPP c++ C )
# force ASM compiler
set( CMAKE_ASM_COMPILER_ID_RUN TRUE )
set( CMAKE_ASM_COMPILER_ID GNU )
set( CMAKE_ASM_COMPILER_WORKS TRUE )
set( CMAKE_ASM_COMPILER_FORCED TRUE )
set( CMAKE_COMPILER_IS_GNUASM 1)
set( CMAKE_ASM_SOURCE_FILE_EXTENSIONS s S asm )

# flags and definitions
set(CMAKE_VERBOSE_MAKEFILE FALSE)
# add_definitions( -DANDROID )
#set( CMAKE_CROSSCOMPILING TRUE )
set(CMAKE_INCLUDE_PATH "${GCC_ROOT}/${MACHINE_NAME}/include" "${CMAKE_INSTALL_PREFIX}/include" /sdcard/C4droid_EXT/include)
set(CMAKE_PROGRAM_PATH "$ENV{PATH}")
set(CMAKE_FRAMEWORK_PATH "${GCC_ROOT}/${MACHINE_NAME}/lib" "${CMAKE_INSTALL_PREFIX}/lib" /sdcard/C4droid_EXT/lib)


set(CMAKE_CXX_FLAGS           ""                    CACHE STRING "c++ flags")
set(CMAKE_C_FLAGS             ""                    CACHE STRING "c flags")
set(CMAKE_SHARED_LINKER_FLAGS ""                    CACHE STRING "shared linker flags")
set(CMAKE_MODULE_LINKER_FLAGS ""                    CACHE STRING "module linker flags")
set(CMAKE_EXE_LINKER_FLAGS    "-Wl,-z,nocopyreloc"  CACHE STRING "executable linker flags")

set(CMAKE_CXX_FLAGS           "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS}" )
set(CMAKE_C_FLAGS             "$ENV{CFLAGS} ${CMAKE_C_FLAGS}" )
set(CMAKE_SHARED_LINKER_FLAGS "$ENV{LDFLAGS} ${CMAKE_SHARED_LINKER_FLAGS}" )
set(CMAKE_MODULE_LINKER_FLAGS "$ENV{LDFLAGS} ${CMAKE_MODULE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS    "$ENV{LDFLAGS} ${CMAKE_EXE_LINKER_FLAGS}")

#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsigned-char -marm -finline-limit=64" ) # good/necessary when porting desktop libraries
#set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsigned-char -marm -finline-limit=64" ) # good/necessary when porting desktop libraries