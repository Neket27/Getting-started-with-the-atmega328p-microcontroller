SET(CMAKE_SYSTEM_NAME Generic)
SET(VISUALGDB_TOOLCHAIN_TYPE Embedded)
SET(VISUALGDB_TOOLCHAIN_SUBTYPE GCC)
if(DEFINED CCACHE_EXE)
SET(CMAKE_C_COMPILER "${CCACHE_EXE}" "$ENV{TOOLCHAIN_ROOT}/bin/avr-gcc.exe")
SET(CMAKE_CXX_COMPILER "${CCACHE_EXE}" "$ENV{TOOLCHAIN_ROOT}/bin/avr-g++.exe")
SET(CMAKE_ASM_COMPILER "${CCACHE_EXE}" "$ENV{TOOLCHAIN_ROOT}/bin/avr-g++.exe")
SET(CMAKE_AR "${TOOLCHAIN_ROOT}/bin/avr-ar.exe")
SET(CMAKE_OBJCOPY "${TOOLCHAIN_ROOT}/bin/avr-objcopy.exe")
else()
SET(CMAKE_C_COMPILER "$ENV{TOOLCHAIN_ROOT}/bin/avr-gcc.exe")
SET(CMAKE_CXX_COMPILER "$ENV{TOOLCHAIN_ROOT}/bin/avr-g++.exe")
SET(CMAKE_ASM_COMPILER "$ENV{TOOLCHAIN_ROOT}/bin/avr-g++.exe")
endif()
SET(CMAKE_LD "${TOOLCHAIN_ROOT}/bin/avr-ld.exe")

if(EXISTS "$ENV{TOOLCHAIN_ROOT}/Qt/v5-CMake/Qt5Cross.cmake")
	include("$ENV{TOOLCHAIN_ROOT}/Qt/v5-CMake/Qt5Cross.cmake")
endif()

#Barebone toolchain cannot link executables without a device-specific linker script
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_OBJECT_PATH_MAX 240)
set(KNOWN_PATH_PREFIX_VARIABLES "" CACHE STRING "Specifies the list of variables like 'BSP_ROOT' that will be substituted by the IDE when referencing source files." FORCE)
set(TOOLCHAIN_HAS_BUILTIN_BSPS 1)
function(visualgdb_toolchain_find_builtin_bsp)
	if("${BSP_ID}" STREQUAL "com.sysprogs.avr.core")
		set(BSP_ROOT "$ENV{TOOLCHAIN_ROOT}/avr-bsp" PARENT_SCOPE)
	endif()
endfunction()

function (visualgdb_toolchain_load_default_configuration)
endfunction() #visualgdb_toolchain_load_default_configuration

function (visualgdb_toolchain_compute_flags)
endfunction() #visualgdb_toolchain_compute_flags



set (VISUALGDB_COMPONENT_LISTS_DIR "C:/Users/Nikita/AppData/Local/VisualGDB")
if (NOT DEFINED VISUALGDB_BUILD_SYSTEM_ROOT)
	set (VISUALGDB_BUILD_SYSTEM_ROOT "C:/Program Files (x86)/Sysprogs/VisualGDB/CMake/embedded")
endif()

include ("${VISUALGDB_BUILD_SYSTEM_ROOT}/root.cmake")
