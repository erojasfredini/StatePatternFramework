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

# Sources
set(PROJECT_HEADERS)
file(GLOB_RECURSE PROJECT_HEADERS RELATIVE ${PROJECT_SOURCE_DIR} *.h *.hpp)
set(PROJECT_SOURCES)
file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} *.c *.cpp)

# Create a library file
add_library(StatePattern STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Headers
set(PROJECT_INCLUDE_DIRS)
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ./Headers/)
#set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} $(BOX2D_INCLUDE))
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} C:/Libraries/Box2D/include)
#set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} $(SFML_INCLUDE))
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} D:/OneDrive-Erojasfredini/OneDrive/Documentos/Libs/SFML/SFML-2.4.2/include)
include_directories(${PROJECT_INCLUDE_DIRS})

# Preprocessor defines
set(DEFINES)
add_compile_definitions(${DEFINES})

# Libraries Dirs
#set(PROJECT_LINK_DIRS)
#set(PROJECT_LINK_DIRS ${PROJECT_LINK_DIRS} "C:/Libraries/Box2D/lib/$<CONFIG>$")
#set(PROJECT_LINK_DIRS ${PROJECT_LINK_DIRS} "C:/Libraries/SFML/lib/$<CONFIG>$")
#link_directories(${PROJECT_LINK_DIRS})

# Libraries
#set(PROJECT_LINK_LIBS)
#set(PROJECT_LINK_LIBS "Box2D")
#target_link_libraries(StatePattern ${PROJECT_LINK_LIBS})

# Output folder
set_target_properties(StatePattern PROPERTIES PDB_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Bin/)
set_target_properties(StatePattern PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Bin/)# ARCHIVE_OUTPUT_DIRECTORY for static libs
#set_target_properties(StatePattern PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)# LIBRARY_OUTPUT_DIRECTORY for dynamic libs
#set_target_properties(StatePattern PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

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