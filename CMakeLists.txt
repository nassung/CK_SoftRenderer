##########################################################
## CMake Setting for SoftRenderer 
##########################################################

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakeDependentOption)
set(MAIN_PROJECT_NAME SoftRenderer)

option(BUILD_RUNTIME "Build Runtime library" OFF)
option(BUILD_PLAYER "Build Player executable" OFF)

if (NOT BUILD_PLAYER)
    message(FATAL_ERROR "no BUILD_PLAYER OPTION")
else()
	message(STATUS "BUILD_PLAYER option detected!")
endif ()

if (BUILD_RUNTIME)
	message(STATUS "BUILD_RUNTIME option detected!")
endif ()

project(${MAIN_PROJECT_NAME})

set(RUNTIME_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/Runtime)
set(PLATFORM_WINDOWS TRUE)

message(STATUS "Running from ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Version: ${CMAKE_VERSION}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}") 
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# DEBUG, _DEBUG definition to compiler in debug build
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

if(!WIN32)
	message(FATAL_ERROR "This project requires windows environment")
endif()

if(MSVC)
	message(STATUS "MSVC Detected")
	
	# Enable Function-Level Linking
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Gy")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gy")

	# Force Synchronous PDB Writes
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /FS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FS")

	# Remove RTTI
	string(REGEX REPLACE "/GR-?" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

	# Suppress warnings when linking with release library
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:\"msvcrt.lib\"")	
else()
	message(FATAL_ERROR "This project requires MSVC")
endif()

################################################################################
# Print C/CXX FLAGS
################################################################################
set(FLAGS
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
)

foreach(FLAG ${FLAGS})
    message(STATUS "${FLAG}: ${${FLAG}}")
endforeach()

################################################################################
# Sub projects
################################################################################

if (BUILD_RUNTIME)
    add_subdirectory(Source/Runtime/Math)
    add_subdirectory(Source/Runtime/Renderer)
    add_subdirectory(Source/Runtime/Engine)
endif ()

if (BUILD_PLAYER)
    add_subdirectory(Source/Player)
endif ()

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SoftRendererPlayer)

