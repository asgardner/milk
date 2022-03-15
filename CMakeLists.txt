set(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(Boost_NO_WARN_NEW_VERSIONS true)
set(ENV{LD_LIBRARY_PATH} "$ENV{HOME}/vcpkg/installed/x64-linux/lib")
enable_testing()
cmake_minimum_required(VERSION 3.14)
project(milk LANGUAGES C CXX)
message("CMake version: ${CMAKE_VERSION}")

option(build_python_module "Compile Python Wrappers" OFF)
if(build_python_module)
add_subdirectory (python_module)
endif()

# Version number
set(VERSION_MAJOR 1)
set(VERSION_MINOR 03)
set(VERSION_PATCH 16)
set(VERSION_OPTION ".chai" )

set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
set(PROJECT_DESCRIPTION "Modular Image processing tooLKit" )
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/milk-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
option(INSTALLMAKEDEFAULT "Make this installation the system default" ON )

# Configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/milk_config.h.in"
  "${PROJECT_SOURCE_DIR}/src/milk_config.h"
  )

configure_file ( 
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_SOURCE_DIR}/src/config.h"
  )

# select most recent version available
SET(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
SET(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)

# main libraries

list(APPEND libsrcdir src/CommandLineInterface)
list(APPEND libname CLIcore)
list(APPEND libsrcdir src/ImageStreamIO)
list(APPEND libname ImageStreamIO m)
list(APPEND modulelist COREMOD_arith COREMOD_iofits COREMOD_memory COREMOD_tools)

foreach(mname IN LISTS modulelist)
  string(REPLACE "_" "" mname1 "${mname}")
  list(APPEND libsrcdir src/${mname})
  list(APPEND libname milk${mname1})
  message("======= adding module  ${mname}")
endforeach()

# =======================================
# BUILD TYPE
# =======================================

# Set Release build flags
# cmake -DCMAKE_BUILD_TYPE=Debug
set(CMAKE_C_FLAGS_DEBUG "-O3 -g -Wall -Wextra \
-march=x86-64 \
-std=c18 \
-fsanitize=undefined -fstack-protector-strong -fstack-protector-all \
-D_POSIX_C_SOURCE=199309L -D_GNU_SOURCE")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g -Wall -Wextra \
-march=x86-64 \
-std=c++20 \
-fsanitize=undefined -fstack-protector-strong -fstack-protector-all \
-D_POSIX_C_SOURCE=199309L -D_GNU_SOURCE")

# Set Release build flags
# cmake -DCMAKE_BUILD_TYPE=Release
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall -Wextra \
-march=skylake-avx512 \
-std=c18 \
-fsanitize=undefined -fstack-protector-strong -fstack-protector-all \
-D_POSIX_C_SOURCE=199309L -D_GNU_SOURCE -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra \
-march=skylake-avx512 \
-std=c++20 \
-fsanitize=undefined -fstack-protector-strong -fstack-protector-all \
-D_POSIX_C_SOURCE=199309L -D_GNU_SOURCE -DNDEBUG")

# Set a default build type if none was specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()

message("CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")

# =======================================
# COMPILE OPTIONS
# =======================================

# link-time optimizer
# add_compile_options($<$<COMPILE_LANGUAGE:C>:-flto>)
# add_compile_options($<$<COMPILE_LANGUAGE:C>:-fwhole-pgram>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-pipe>)

message("COMPILE_OPTIONS:  ${COMPILE_OPTIONS}")
message("====================================================")
message("VERSION  ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message("====================================================")
message("CMAKE_THREAD_LIBS_INIT  :  ${CMAKE_THREAD_LIBS_INIT}")
message("PROJECT_SOURCE_DIR       = ${PROJECT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR       = ${PROJECT_BINARY_DIR}")
message("CMAKE_INSTALL_PREFIX     = ${CMAKE_INSTALL_PREFIX}")
message("EXTRAMODULES             = ${EXTRAMODULES}")

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# ADD SOURCE DIRECTORIES
# recursively add libraries to be compiled
execute_process(COMMAND bash -c "find ${PROJECT_SOURCE_DIR}/plugins -mindepth 2 -maxdepth 2 -type d | fgrep -v .git" OUTPUT_VARIABLE libextradir)
string(REGEX REPLACE "\n" "; " libextradir "${libextradir}")

foreach(lsrcdir IN LISTS libsrcdir libextradir)
  string(REGEX REPLACE " " "" lsrcdir "${lsrcdir}")
  if (NOT ${lsrcdir} EQUAL "")
    add_subdirectory ( ${lsrcdir} )
  endif()
endforeach()

#####
# main

# Packages from vcpkg.
find_package(Boost REQUIRED COMPONENTS system thread unit_test_framework)
add_executable(milk src/CLImain.cc)
target_link_directories(milk PUBLIC $ENV{HOME}/vcpkg/installed/x64-linux/lib)
target_link_libraries(milk PUBLIC 
  ${libname}
  ${Boost_SYSTEM_LIBRARY} 
  ${Boost_THREAD_LIBRARY}
  boost_atomic
  boost_date_time
)
set(BUILD_FLAGS "-DPACKAGE_NAME=\\\"milk\\\" -DCONFIGDIR=\\\"${PROJECT_SOURCE_DIR}/config\\\" -DINSTALLDIR=\\\"${CMAKE_INSTALL_PREFIX}\\\" -DSOURCEDIR=\\\"${PROJECT_SOURCE_DIR}\\\" -DABSSRCTOPDIR=\\\"${PROJECT_SOURCE_DIR}\\\" -DPACKAGE_BUGREPORT=\\\"https://github.com/milk-org/milk/issues\\\"" )
set_target_properties(milk PROPERTIES COMPILE_FLAGS "${BUILD_FLAGS}" )

message("====================================================")
get_target_property(milkcompopts milk COMPILE_OPTIONS)

message("BUILD_FLAGS             :  ${BUILD_FLAGS}")
message("COMPILE_FLAGS           :  ${COMPILE_FLAGS}")
message("COMPILE_OPTIONS         :  ${milkcompopts}")
message("CMAKE_EXE_LINKER_FLAGS  :  ${CMAKE_EXE_LINKER_FLAGS}")

message("CMAKE_C_FLAGS           :  ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS         :  ${CMAKE_CXX_FLAGS}")

message("CMAKE_C_FLAGS_DEBUG           : ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE         : ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_RELWITHDEBINFO  : ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message("CMAKE_C_FLAGS_MINSIZEREL      : ${CMAKE_C_FLAGS_MINSIZEREL}")

message("CMAKE_CURRENT_SOURCE_DIR      : ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR      : ${CMAKE_CURRENT_BINARY_DIR}")
message("INSTALL_PKGCONFIG_DIR         : ${INSTALL_PKGCONFIG_DIR}")
message("====================================================")

set(PROGRAM_PERMISSIONS_DEFAULT
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

install(TARGETS milk DESTINATION bin PERMISSIONS ${PROGRAM_PERMISSIONS_DEFAULT} SETUID)

# =======================================
# MAKE DEFAULT
# =======================================

set(CMAKE_INSTALL_PREFIX_LINK "/usr/local/${PROJECT_NAME}")
if(INSTALLMAKEDEFAULT)
    install(CODE "
    EXECUTE_PROCESS(COMMAND ln -snf ${CMAKE_INSTALL_PREFIX} ${CMAKE_INSTALL_PREFIX_LINK})
    ")
endif(INSTALLMAKEDEFAULT)

# =======================================
# PKG-CONFIG
# =======================================

foreach(lname IN LISTS libname)
string(APPEND LINKSTRING "-l${lname} ")
endforeach()

message("LINKSTRING: ${LINKSTRING}")

set(INSTALL_PKGCONFIG_DIR "lib/pkgconfig"
    CACHE PATH "Installation directory for pkgconfig (.pc) files")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/milk.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/milk.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/milk.pc
        DESTINATION "${INSTALL_PKGCONFIG_DIR}")


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/milk_config.h
              ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h
        DESTINATION include
        )

install(TARGETS CLIcore DESTINATION bin)

install(CODE "execute_process(
  COMMAND ls -1rt /usr/local/ 
  COMMAND fgrep milk
  COMMAND tail -1
  COMMAND xargs -iQ ln -svf /usr/local/Q/bin/chai /usr/local/bin/chai
  ECHO_OUTPUT_VARIABLE OUTPUT_VARIABLE OUTPUT)")

install(CODE "execute_process(
  COMMAND ls -1rt /usr/local/ 
  COMMAND fgrep milk
  COMMAND tail -1
  COMMAND xargs -iQ ln -svf /usr/local/Q/bin/milk /usr/local/bin/milk
  ECHO_OUTPUT_VARIABLE OUTPUT_VARIABLE OUTPUT)")
