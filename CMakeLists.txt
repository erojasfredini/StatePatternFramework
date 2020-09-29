####################################################################################
# Welcome to the CMake build system for StatePattern Lib.                          #
# This is the main file where the general build environment is set-up and the      #
# the build configuration options are initialized.                                 #
####################################################################################

cmake_minimum_required(VERSION 3.15.0)

##################################################################
#############               SOLUTION                 #############
##################################################################

project(StatePatternLibrary LANGUAGES CXX)

##################################################################
#############               PROJECT                  #############
##################################################################

set(CMAKE_BUILD_TYPE Release)

# Find SFML
## If you want to link SFML statically
#set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_STATIC_LIBRARIES FALSE)
set(SFML_FIND_PATH ${CMAKE_PREFIX_PATH})
set(SFML_INCLUDE_PATH "${SFML_FIND_PATH}/include")
find_package(SFML 2.5 REQUIRED system window graphics audio)

# Find Box2D
set(BOX2D_FIND_PATH ${CMAKE_PREFIX_PATH})
set(BOX2D_INCLUDE_PATH "${BOX2D_FIND_PATH}/include")
find_package(box2d REQUIRED)

# Headers
set(PROJECT_INCLUDE_DIRS)
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ./Headers/)
include_directories(${PROJECT_INCLUDE_DIRS})

# Preprocessor defines
set(DEFINES)
add_compile_definitions(${DEFINES})

# Sources
set(PROJECT_HEADERS)
file(GLOB_RECURSE PROJECT_HEADERS RELATIVE ${PROJECT_SOURCE_DIR} *.h *.hpp)
set(PROJECT_SOURCES)
file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} *.c *.cpp)

# Create a library file
add_library(StatePattern STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Libraries
set(PROJECT_LINK_LIBS)
set(PROJECT_LINK_LIBS "sfml-main" "sfml-system" "sfml-graphics" "sfml-window" "sfml-audio")

target_link_libraries(StatePattern PRIVATE box2d::box2d ${PROJECT_LINK_LIBS})

# Output folder
set_target_properties(StatePattern PROPERTIES PDB_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Bin/)
set_target_properties(StatePattern PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Bin/)# ARCHIVE_OUTPUT_DIRECTORY for static libs

# Set compiler to C++17
set_property(TARGET StatePattern PROPERTY CXX_STANDARD 17)

##################################################################
#############           GENERAL CONFIG               #############
##################################################################

# Make CXX_STANDARD version a requirement (it won't compile with older version)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Visual Studio specific config
if (MSVC)
    # Multiprocesor compilation
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" CACHE STRING "compile flags" FORCE)
endif ()